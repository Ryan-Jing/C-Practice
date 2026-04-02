/**************************************************************************************************/
/**
 * @file player_state.mm
 * @brief Reports whether the macOS system has an active now-playing session and whether it is
 *        currently playing.
 *
 * To build:

    clang++ -std=c++17 -Wall -Wextra -fobjc-arc \
    -framework Foundation \
    volume/player_state.mm -o volume/player_state

 */
/**************************************************************************************************/

#include <dlfcn.h>
#include <stdio.h>

#import <Foundation/Foundation.h>
#import <dispatch/dispatch.h>
#import <objc/message.h>

typedef void (*MRGetPlaybackStateFn)(dispatch_queue_t, void (^)(unsigned int));
typedef void (*MRGetNowPlayingInfoFn)(dispatch_queue_t, void (^)(CFDictionaryRef));

static const unsigned int kMRPlaybackStatePlaying = 2;

int main(void)
{
    @autoreleasepool {
        NSBundle *bundle =
            [NSBundle bundleWithPath:@"/System/Library/PrivateFrameworks/MediaRemote.framework"];
        if (bundle == nil || ![bundle load]) {
            printf("{\"active\":false,\"playing\":false}\n");
            return 1;
        }

        MRGetPlaybackStateFn getPlaybackState = (MRGetPlaybackStateFn)dlsym(
            RTLD_DEFAULT,
            "MRMediaRemoteGetNowPlayingApplicationPlaybackState");
        MRGetNowPlayingInfoFn getNowPlayingInfo = (MRGetNowPlayingInfoFn)dlsym(
            RTLD_DEFAULT,
            "MRMediaRemoteGetNowPlayingInfo");

        Class requestClass = NSClassFromString(@"MRNowPlayingRequest");
        SEL playerPathSelector = NSSelectorFromString(@"localNowPlayingPlayerPath");
        if (getPlaybackState == NULL || getNowPlayingInfo == NULL || requestClass == Nil ||
            ![requestClass respondsToSelector:playerPathSelector]) {
            printf("{\"active\":false,\"playing\":false}\n");
            return 1;
        }

        id (*sendObjectMessage)(id, SEL) = (id (*)(id, SEL))objc_msgSend;
        (void)sendObjectMessage(requestClass, playerPathSelector);

        dispatch_queue_t queue =
            dispatch_queue_create("com.ryanjing.sketchybar.audio-viz.player-state",
                                  DISPATCH_QUEUE_SERIAL);

        dispatch_semaphore_t stateSemaphore = dispatch_semaphore_create(0);
        dispatch_semaphore_t infoSemaphore = dispatch_semaphore_create(0);

        __block unsigned int playbackState = UINT_MAX;
        __block NSDictionary *nowPlayingInfo = nil;

        getPlaybackState(queue, ^(unsigned int value) {
            playbackState = value;
            dispatch_semaphore_signal(stateSemaphore);
        });

        getNowPlayingInfo(queue, ^(CFDictionaryRef value) {
            NSDictionary *dictionary = (__bridge NSDictionary *)value;
            nowPlayingInfo = dictionary != nil ? [dictionary copy] : nil;
            dispatch_semaphore_signal(infoSemaphore);
        });

        dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC));
        const bool gotState = dispatch_semaphore_wait(stateSemaphore, timeout) == 0;
        const bool gotInfo = dispatch_semaphore_wait(infoSemaphore, timeout) == 0;
        const bool hasNowPlayingInfo = gotInfo && nowPlayingInfo != nil && nowPlayingInfo.count > 0;
        const bool playing = gotState && playbackState == kMRPlaybackStatePlaying;
        const bool active = hasNowPlayingInfo || playing;

        printf("{\"active\":%s,\"playing\":%s}\n",
               active ? "true" : "false",
               (active && playing) ? "true" : "false");

        return 0;
    }
}
