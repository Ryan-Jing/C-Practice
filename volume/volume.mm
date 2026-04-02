/**************************************************************************************************/
/**
 * @file volume.mm
 * @author Ryan Jing
 * @brief Captures the system output mix, computes four FFT bands, and updates SketchyBar.
 *
 * This helper is intended to run as a long-lived process behind a SketchyBar widget.
 *
 * The scope of this project was larger than expected, much of this was developed in tandem
 * with AI tools.
 *
 * To build:

    clang++ -std=c++17 -Wall -Wextra -fobjc-arc \
    -framework Foundation \
    -framework CoreAudio \
    -framework AudioToolbox \
    -framework Accelerate \
    volume/volume.mm -o volume/volume

 */
/**************************************************************************************************/

#include <math.h>
#include <signal.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#import <Accelerate/Accelerate.h>
#import <AudioToolbox/AudioToolbox.h>
#import <CoreAudio/AudioHardwareTapping.h>
#import <CoreAudio/CATapDescription.h>
#import <CoreAudio/CoreAudio.h>
#import <Foundation/Foundation.h>

extern char **environ;

#if __has_feature(objc_arc)
#define CF_BRIDGE_CAST(type, value) ((__bridge type)(value))
#else
#define CF_BRIDGE_CAST(type, value) ((type)(value))
#endif

#define FFT_SIZE       1024
#define LOG2_FFT       10
#define NUM_BANDS      4
#define UPDATE_FPS     10
#define MIN_BAR_HEIGHT 2
#define MAX_BAR_HEIGHT 14

static const char *kPIDFilePath = "/tmp/sketchybar_audio_viz.pid";
static const float kBandRangesHz[NUM_BANDS][2] = {
    { 40.0f,   250.0f },
    { 250.0f, 2000.0f },
    { 2000.0f, 6000.0f },
    { 6000.0f, 20000.0f }
};

static const UInt32 kHALInputBus = 1;
static const UInt32 kHALOutputBus = 0;

static const float kBandGain[NUM_BANDS]         = { 0.22f, 0.32f, 0.42f, 0.52f };
static const float kBandFloorDB[NUM_BANDS]      = { -35.0f, -38.0f, -45.0f, -45.0f };
static const float kBandCeilingDB[NUM_BANDS]    = {   2.0f,   5.0f,  15.0f,  15.0f };
static const float kBandHeightScale[NUM_BANDS]  = { 0.85f, 1.0f, 1.0f, 1.0f };
static const float kResponsePivot               = 0.35f; //where the curve switches from suppressing quiet to boosting loud
static const float kQuietExponent               = 1.8f; // higher makes quiet audio flatter
static const float kLoudExponent                = 0.75f; // lower makes loud audio jump more

static volatile sig_atomic_t g_running = 1;
static float g_bands[NUM_BANDS] = { 0.0f };

typedef struct AudioVizState {
    AudioObjectID tapID;
    AudioObjectID aggregateDeviceID;
    AudioUnit audioUnit;
    Float64 sampleRate;
    UInt32 captureCapacityFrames;
    float *captureSamples;
    AudioBufferList captureBufferList;

    FFTSetup fftSetup;
    float window[FFT_SIZE];
    float analysisBuffer[FFT_SIZE];
    float windowedBuffer[FFT_SIZE];
    float magnitudes[FFT_SIZE / 2];
    float splitReal[FFT_SIZE / 2];
    float splitImag[FFT_SIZE / 2];
    DSPSplitComplex splitComplex;
    UInt32 analysisFillCount;
    int bandLow[NUM_BANDS];
    int bandHigh[NUM_BANDS];
} AudioVizState;

static NSString *CAKey(const char *key)
{
    return [NSString stringWithUTF8String:key];
}

static float clampfValue(float value, float minimum, float maximum)
{
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

static float shapeResponse(float normalizedLevel)
{
    normalizedLevel = clampfValue(normalizedLevel, 0.0f, 1.0f);

    if (normalizedLevel <= 0.0f || normalizedLevel >= 1.0f) {
        return normalizedLevel;
    }

    if (normalizedLevel < kResponsePivot) {
        const float t = normalizedLevel / kResponsePivot;
        return kResponsePivot * powf(t, kQuietExponent);
    }

    const float t = (normalizedLevel - kResponsePivot) / (1.0f - kResponsePivot);
    return kResponsePivot + (1.0f - kResponsePivot) * powf(t, kLoudExponent);
}

static void handleSignal(int signalNumber)
{
    (void)signalNumber;
    g_running = 0;
}

static void logOSStatus(const char *operation, OSStatus status)
{
    UInt32 code = CFSwapInt32HostToBig((UInt32)status);
    char fourCC[5] = { 0 };
    memcpy(fourCC, &code, sizeof(code));

    bool printable = true;
    for (size_t i = 0; i < sizeof(code); ++i) {
        if (fourCC[i] < 32 || fourCC[i] > 126) {
            printable = false;
            break;
        }
    }

    if (printable) {
        fprintf(stderr, "%s failed: %d ('%s')\n", operation, (int)status, fourCC);
    } else {
        fprintf(stderr, "%s failed: %d\n", operation, (int)status);
    }
}

static bool checkStatus(OSStatus status, const char *operation)
{
    if (status == noErr) {
        return true;
    }

    logOSStatus(operation, status);
    return false;
}

static bool writePIDFile(void)
{
    FILE *pidFile = fopen(kPIDFilePath, "w");
    if (pidFile == NULL) {
        perror("fopen PID file");
        return false;
    }

    fprintf(pidFile, "%d\n", getpid());
    fclose(pidFile);
    return true;
}

static void removePIDFile(void)
{
    unlink(kPIDFilePath);
}

static const char *resolveSketchyBarBinary(void)
{
    static const char *resolvedPath = NULL;
    if (resolvedPath != NULL) {
        return resolvedPath;
    }

    const char *envPath = getenv("SKETCHYBAR_BIN");
    if (envPath != NULL && access(envPath, X_OK) == 0) {
        resolvedPath = envPath;
        return resolvedPath;
    }

    static const char *candidates[] = {
        "/opt/homebrew/bin/sketchybar",
        "/usr/local/bin/sketchybar",
        "sketchybar"
    };

    for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); ++i) {
        if (candidates[i][0] == '/' && access(candidates[i], X_OK) == 0) {
            resolvedPath = candidates[i];
            return resolvedPath;
        }
    }

    resolvedPath = "sketchybar";
    return resolvedPath;
}

static void updateSketchyBarHeights(int h1, int h2, int h3, int h4)
{
    static bool warnedOnce = false;

    char height1[32];
    char height2[32];
    char height3[32];
    char height4[32];
    snprintf(height1, sizeof(height1), "background.height=%d", h1);
    snprintf(height2, sizeof(height2), "background.height=%d", h2);
    snprintf(height3, sizeof(height3), "background.height=%d", h3);
    snprintf(height4, sizeof(height4), "background.height=%d", h4);

    const char *binary = resolveSketchyBarBinary();
    char *const argv[] = {
        (char *)binary,
        (char *)"--set", (char *)"audio.bar1", height1,
        (char *)"--set", (char *)"audio.bar2", height2,
        (char *)"--set", (char *)"audio.bar3", height3,
        (char *)"--set", (char *)"audio.bar4", height4,
        NULL
    };

    pid_t child = 0;
    int spawnStatus = (binary[0] == '/')
        ? posix_spawn(&child, binary, NULL, NULL, argv, environ)
        : posix_spawnp(&child, binary, NULL, NULL, argv, environ);

    if (spawnStatus != 0) {
        if (!warnedOnce) {
            fprintf(stderr, "Unable to execute sketchybar (%s): %s\n", binary, strerror(spawnStatus));
            warnedOnce = true;
        }
        return;
    }

    int waitStatus = 0;
    (void)waitpid(child, &waitStatus, 0);
}

static void computeBandBins(AudioVizState *state)
{
    const float binWidth = (float)(state->sampleRate / (Float64)FFT_SIZE);

    for (int band = 0; band < NUM_BANDS; ++band) {
        int low = (int)floorf(kBandRangesHz[band][0] / binWidth);
        int high = (int)floorf(kBandRangesHz[band][1] / binWidth);

        low = (int)clampfValue((float)low, 1.0f, (float)(FFT_SIZE / 2 - 1));
        high = (int)clampfValue((float)high, (float)low, (float)(FFT_SIZE / 2 - 1));

        state->bandLow[band] = low;
        state->bandHigh[band] = high;
    }
}

static void analyzeWindow(AudioVizState *state)
{
    vDSP_vmul(state->analysisBuffer, 1, state->window, 1, state->windowedBuffer, 1, FFT_SIZE);
    vDSP_ctoz((DSPComplex *)state->windowedBuffer, 2, &state->splitComplex, 1, FFT_SIZE / 2);
    vDSP_fft_zrip(state->fftSetup, &state->splitComplex, 1, LOG2_FFT, FFT_FORWARD);
    vDSP_zvmags(&state->splitComplex, 1, state->magnitudes, 1, FFT_SIZE / 2);

    for (int band = 0; band < NUM_BANDS; ++band) {
        const int low = state->bandLow[band];
        const int high = state->bandHigh[band];
        float sum = 0.0f;

        for (int bin = low; bin <= high; ++bin) {
            sum += state->magnitudes[bin];
        }

        const float averagePower = sum / (float)(high - low + 1);
        const float decibels = 10.0f * log10f(averagePower + 1.0e-12f);

        float normalized =
            ((decibels - kBandFloorDB[band]) /
            (kBandCeilingDB[band] - kBandFloorDB[band])) * 10.0f;

        normalized *= kBandGain[band];
        g_bands[band] = shapeResponse(normalized / 10.0f) * 10.0f;
    }
}

static OSStatus renderCallback(void *inRefCon,
                               AudioUnitRenderActionFlags *flags,
                               const AudioTimeStamp *timestamp,
                               UInt32 busNumber,
                               UInt32 numFrames,
                               AudioBufferList *bufferList)
{
    (void)bufferList;
    (void)busNumber;

    AudioVizState *state = (AudioVizState *)inRefCon;
    if (state == NULL || state->audioUnit == NULL || state->captureSamples == NULL) {
        return noErr;
    }

    if (numFrames > state->captureCapacityFrames) {
        return kAudioHardwareBadPropertySizeError;
    }

    state->captureBufferList.mNumberBuffers = 1;
    state->captureBufferList.mBuffers[0].mNumberChannels = 1;
    state->captureBufferList.mBuffers[0].mDataByteSize = numFrames * sizeof(float);
    state->captureBufferList.mBuffers[0].mData = state->captureSamples;

    OSStatus status = AudioUnitRender(state->audioUnit,
                                      flags,
                                      timestamp,
                                      kHALInputBus,
                                      numFrames,
                                      &state->captureBufferList);
    if (status != noErr) {
        return status;
    }

    for (UInt32 frame = 0; frame < numFrames; ++frame) {
        state->analysisBuffer[state->analysisFillCount++] = state->captureSamples[frame];

        if (state->analysisFillCount == FFT_SIZE) {
            analyzeWindow(state);
            state->analysisFillCount = 0;
        }
    }

    return noErr;
}

static OSStatus getDeviceSampleRate(AudioObjectID deviceID, Float64 *sampleRate)
{
    AudioObjectPropertyAddress propertyAddress = {
        kAudioDevicePropertyNominalSampleRate,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };

    UInt32 propertySize = sizeof(*sampleRate);
    return AudioObjectGetPropertyData(deviceID,
                                      &propertyAddress,
                                      0,
                                      NULL,
                                      &propertySize,
                                      sampleRate);
}

static OSStatus getDefaultOutputDevice(AudioObjectID *deviceID)
{
    AudioObjectPropertyAddress propertyAddress = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };

    UInt32 propertySize = sizeof(*deviceID);
    return AudioObjectGetPropertyData(kAudioObjectSystemObject,
                                      &propertyAddress,
                                      0,
                                      NULL,
                                      &propertySize,
                                      deviceID);
}

static NSString *copyDeviceUIDString(AudioObjectID deviceID)
{
    AudioObjectPropertyAddress propertyAddress = {
        kAudioDevicePropertyDeviceUID,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };

    CFStringRef deviceUID = NULL;
    UInt32 propertySize = sizeof(deviceUID);
    OSStatus status = AudioObjectGetPropertyData(deviceID,
                                                 &propertyAddress,
                                                 0,
                                                 NULL,
                                                 &propertySize,
                                                 &deviceUID);
    if (status != noErr || deviceUID == NULL) {
        logOSStatus("AudioObjectGetPropertyData(kAudioDevicePropertyDeviceUID)", status);
        return nil;
    }

    NSString *deviceUIDString = [NSString stringWithString:CF_BRIDGE_CAST(NSString *, deviceUID)];
    CFRelease(deviceUID);
    return deviceUIDString;
}

static NSString *copyTapUIDString(AudioObjectID tapID)
{
    AudioObjectPropertyAddress propertyAddress = {
        kAudioTapPropertyUID,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };

    CFStringRef tapUID = NULL;
    UInt32 propertySize = sizeof(tapUID);
    OSStatus status = AudioObjectGetPropertyData(tapID,
                                                 &propertyAddress,
                                                 0,
                                                 NULL,
                                                 &propertySize,
                                                 &tapUID);
    if (status != noErr || tapUID == NULL) {
        return nil;
    }

    NSString *tapUIDString = [NSString stringWithString:CF_BRIDGE_CAST(NSString *, tapUID)];
    CFRelease(tapUID);
    return tapUIDString;
}

static OSStatus setupAudioTap(AudioVizState *state)
{
    if (@available(macOS 14.2, *)) {
        // Process tap support is available.
    } else {
        fprintf(stderr, "Process taps require macOS 14.2 or newer.\n");
        return kAudioHardwareUnsupportedOperationError;
    }

    CATapDescription *tapDescription =
        [[CATapDescription alloc] initMonoGlobalTapButExcludeProcesses:@[]];
    if (tapDescription == nil) {
        return kAudioHardwareUnspecifiedError;
    }

    tapDescription.name = @"SketchyBar Audio Visualizer Tap";
    tapDescription.UUID = [NSUUID UUID];
    tapDescription.privateTap = YES;
    tapDescription.muteBehavior = CATapUnmuted;

    OSStatus status = AudioHardwareCreateProcessTap(tapDescription, &state->tapID);
    if (status != noErr) {
        logOSStatus("AudioHardwareCreateProcessTap", status);
        fprintf(stderr, "Creating the audio tap usually requires Screen & System Audio Recording permission.\n");
#if !__has_feature(objc_arc)
        [tapDescription release];
#endif
        return status;
    }

    AudioObjectID defaultOutputDevice = kAudioObjectUnknown;
    NSString *defaultOutputUID = nil;
    status = getDefaultOutputDevice(&defaultOutputDevice);
    if (status == noErr && defaultOutputDevice != kAudioObjectUnknown) {
        defaultOutputUID = copyDeviceUIDString(defaultOutputDevice);
    }

    NSDictionary *tapEntry = @{
        CAKey(kAudioSubTapUIDKey): copyTapUIDString(state->tapID) ?: tapDescription.UUID.UUIDString,
        CAKey(kAudioSubTapDriftCompensationKey): @YES
    };

    NSDictionary *aggregateDescription = nil;
    if (defaultOutputUID != nil) {
        NSDictionary *subDeviceEntry = @{
            CAKey(kAudioSubDeviceUIDKey): defaultOutputUID
        };

        aggregateDescription = @{
            CAKey(kAudioAggregateDeviceNameKey): @"SketchyBar Audio Visualizer",
            CAKey(kAudioAggregateDeviceUIDKey):
                [NSString stringWithFormat:@"com.ryanjing.sketchybar.audio-viz.%@",
                                           [NSUUID UUID].UUIDString],
            CAKey(kAudioAggregateDeviceIsPrivateKey): @YES,
            CAKey(kAudioAggregateDeviceIsStackedKey): @NO,
            CAKey(kAudioAggregateDeviceMainSubDeviceKey): defaultOutputUID,
            CAKey(kAudioAggregateDeviceSubDeviceListKey): @[ subDeviceEntry ],
            CAKey(kAudioAggregateDeviceTapAutoStartKey): @YES,
            CAKey(kAudioAggregateDeviceTapListKey): @[ tapEntry ]
        };
    } else {
        aggregateDescription = @{
            CAKey(kAudioAggregateDeviceNameKey): @"SketchyBar Audio Visualizer",
            CAKey(kAudioAggregateDeviceUIDKey):
                [NSString stringWithFormat:@"com.ryanjing.sketchybar.audio-viz.%@",
                                           [NSUUID UUID].UUIDString],
            CAKey(kAudioAggregateDeviceIsPrivateKey): @YES,
            CAKey(kAudioAggregateDeviceTapAutoStartKey): @YES,
            CAKey(kAudioAggregateDeviceTapListKey): @[ tapEntry ]
        };
    }

#if !__has_feature(objc_arc)
    [tapDescription release];
#endif

    status = AudioHardwareCreateAggregateDevice(
        CF_BRIDGE_CAST(CFDictionaryRef, aggregateDescription),
        &state->aggregateDeviceID);
    if (status != noErr) {
        logOSStatus("AudioHardwareCreateAggregateDevice", status);
        return status;
    }

    state->sampleRate = 44100.0;
    status = getDeviceSampleRate(state->aggregateDeviceID, &state->sampleRate);
    if (status != noErr || state->sampleRate <= 0.0) {
        state->sampleRate = 44100.0;
    }

    AudioComponentDescription componentDescription = {
        .componentType = kAudioUnitType_Output,
        .componentSubType = kAudioUnitSubType_HALOutput,
        .componentManufacturer = kAudioUnitManufacturer_Apple,
        .componentFlags = 0,
        .componentFlagsMask = 0
    };

    AudioComponent component = AudioComponentFindNext(NULL, &componentDescription);
    if (component == NULL) {
        return kAudioHardwareUnsupportedOperationError;
    }

    status = AudioComponentInstanceNew(component, &state->audioUnit);
    if (status != noErr) {
        logOSStatus("AudioComponentInstanceNew", status);
        return status;
    }

    UInt32 enable = 1;
    UInt32 disable = 0;

    status = AudioUnitSetProperty(state->audioUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Input,
                                  kHALInputBus,
                                  &enable,
                                  sizeof(enable));
    if (status != noErr) {
        logOSStatus("Enable HAL input", status);
        return status;
    }

    status = AudioUnitSetProperty(state->audioUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Output,
                                  kHALOutputBus,
                                  &disable,
                                  sizeof(disable));
    if (status != noErr) {
        logOSStatus("Disable HAL output", status);
        return status;
    }

    status = AudioUnitSetProperty(state->audioUnit,
                                  kAudioOutputUnitProperty_CurrentDevice,
                                  kAudioUnitScope_Global,
                                  0,
                                  &state->aggregateDeviceID,
                                  sizeof(state->aggregateDeviceID));
    if (status != noErr) {
        logOSStatus("Set HAL current device", status);
        return status;
    }

    AudioStreamBasicDescription streamFormat = {};
    streamFormat.mSampleRate = state->sampleRate;
    streamFormat.mFormatID = kAudioFormatLinearPCM;
    streamFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
    streamFormat.mBytesPerPacket = sizeof(float);
    streamFormat.mFramesPerPacket = 1;
    streamFormat.mBytesPerFrame = sizeof(float);
    streamFormat.mChannelsPerFrame = 1;
    streamFormat.mBitsPerChannel = 8 * sizeof(float);

    status = AudioUnitSetProperty(state->audioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Output,
                                  kHALInputBus,
                                  &streamFormat,
                                  sizeof(streamFormat));
    if (status != noErr) {
        logOSStatus("Set HAL stream format", status);
        return status;
    }

    AURenderCallbackStruct callbackStruct = {
        .inputProc = renderCallback,
        .inputProcRefCon = state
    };

    status = AudioUnitSetProperty(state->audioUnit,
                                  kAudioOutputUnitProperty_SetInputCallback,
                                  kAudioUnitScope_Global,
                                  0,
                                  &callbackStruct,
                                  sizeof(callbackStruct));
    if (status != noErr) {
        logOSStatus("Set HAL input callback", status);
        return status;
    }

    status = AudioUnitInitialize(state->audioUnit);
    if (status != noErr) {
        logOSStatus("AudioUnitInitialize", status);
        return status;
    }

    UInt32 maxFramesPerSlice = 0;
    UInt32 propertySize = sizeof(maxFramesPerSlice);
    status = AudioUnitGetProperty(state->audioUnit,
                                  kAudioUnitProperty_MaximumFramesPerSlice,
                                  kAudioUnitScope_Global,
                                  0,
                                  &maxFramesPerSlice,
                                  &propertySize);
    if (status != noErr || maxFramesPerSlice == 0) {
        maxFramesPerSlice = FFT_SIZE;
    }

    state->captureCapacityFrames = maxFramesPerSlice;
    state->captureSamples = (float *)calloc(state->captureCapacityFrames, sizeof(float));
    if (state->captureSamples == NULL) {
        return memFullErr;
    }

    state->captureBufferList.mNumberBuffers = 1;
    state->captureBufferList.mBuffers[0].mNumberChannels = 1;
    state->captureBufferList.mBuffers[0].mDataByteSize =
        state->captureCapacityFrames * sizeof(float);
    state->captureBufferList.mBuffers[0].mData = state->captureSamples;

    computeBandBins(state);

    status = AudioOutputUnitStart(state->audioUnit);
    if (status != noErr) {
        logOSStatus("AudioOutputUnitStart", status);
    }

    return status;
}

static void teardownAudioTap(AudioVizState *state)
{
    if (state->audioUnit != NULL) {
        AudioOutputUnitStop(state->audioUnit);
        AudioUnitUninitialize(state->audioUnit);
        AudioComponentInstanceDispose(state->audioUnit);
        state->audioUnit = NULL;
    }

    if (state->aggregateDeviceID != kAudioObjectUnknown) {
        AudioHardwareDestroyAggregateDevice(state->aggregateDeviceID);
        state->aggregateDeviceID = kAudioObjectUnknown;
    }

    if (state->tapID != kAudioObjectUnknown) {
        if (@available(macOS 14.2, *)) {
            AudioHardwareDestroyProcessTap(state->tapID);
        }
        state->tapID = kAudioObjectUnknown;
    }

    free(state->captureSamples);
    state->captureSamples = NULL;
    state->captureCapacityFrames = 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    @autoreleasepool {
        signal(SIGTERM, handleSignal);
        signal(SIGINT, handleSignal);

        AudioVizState state = {};
        state.tapID = kAudioObjectUnknown;
        state.aggregateDeviceID = kAudioObjectUnknown;
        state.splitComplex.realp = state.splitReal;
        state.splitComplex.imagp = state.splitImag;

        state.fftSetup = vDSP_create_fftsetup(LOG2_FFT, kFFTRadix2);
        if (state.fftSetup == NULL) {
            fprintf(stderr, "Unable to create FFT setup.\n");
            return 1;
        }

        vDSP_hann_window(state.window, FFT_SIZE, vDSP_HANN_NORM);

        if (!writePIDFile()) {
            vDSP_destroy_fftsetup(state.fftSetup);
            return 1;
        }

        OSStatus status = setupAudioTap(&state);
        if (!checkStatus(status, "setupAudioTap")) {
            updateSketchyBarHeights(MIN_BAR_HEIGHT,
                                    MIN_BAR_HEIGHT,
                                    MIN_BAR_HEIGHT,
                                    MIN_BAR_HEIGHT);
            teardownAudioTap(&state);
            vDSP_destroy_fftsetup(state.fftSetup);
            removePIDFile();
            return 1;
        }

        float smoothed[NUM_BANDS] = { 0.0f };
        int lastHeights[NUM_BANDS] = { -1, -1, -1, -1 };
        while (g_running) {
            int heights[NUM_BANDS];
            bool changed = false;

            for (int band = 0; band < NUM_BANDS; ++band) {
                const float target = g_bands[band];

                if (target > smoothed[band]) {
                    smoothed[band] += (target - smoothed[band]) * 0.80f;
                } else {
                    smoothed[band] += (target - smoothed[band]) * 0.25f;
                }

                heights[band] = (int)lroundf(
                    clampfValue(smoothed[band] * 2.5f * kBandHeightScale[band],
                    (float)MIN_BAR_HEIGHT,
                    (float)MAX_BAR_HEIGHT));

                if (heights[band] != lastHeights[band]) {
                    changed = true;
                }
            }

            if (changed) {
                updateSketchyBarHeights(heights[0], heights[1], heights[2], heights[3]);
                memcpy(lastHeights, heights, sizeof(lastHeights));
            }

            usleep((useconds_t)(1000000 / UPDATE_FPS));
        }

        updateSketchyBarHeights(MIN_BAR_HEIGHT,
                                MIN_BAR_HEIGHT,
                                MIN_BAR_HEIGHT,
                                MIN_BAR_HEIGHT);
        teardownAudioTap(&state);
        vDSP_destroy_fftsetup(state.fftSetup);
        removePIDFile();
    }

    return 0;
}
