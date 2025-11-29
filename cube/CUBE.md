# Spinning Cube Animation
> Following project: https://github.com/tarantino07/cube.c

## Table of Contents
1. C Program Explanation
2. Build Systems (Make vs CMake)
3. Memory Safety Analysis
4. Rotation Matrix Mathematics

---

## 1. C Program Explanation

### Header Includes and Platform Compatibility
```c
#include <math.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec) { /* Windows implementation */ }
#endif
```
**Purpose:** Cross-platform compatibility
- `math.h` - Provides sin(), cos() functions for rotation
- `stdio.h` - Standard input/output (printf, putchar)
- `string.h` - Memory operations (memset)
- `unistd.h` (Unix/Linux) - Provides usleep() for timing
- Windows alternative - Implements usleep() using Windows API since it doesn't exist natively

### Global Variables
```c
float A, B, C;  // Rotation angles around X, Y, Z axes
```
These angles increment each frame to create continuous rotation.

```c
float cubeWidth = 20;  // Half-width of the cube
int width = 160, height = 44;  // Terminal dimensions in characters
```
Screen resolution for the ASCII canvas.

```c
float zBuffer[160 * 44];  // Depth buffer (7,040 floats)
char buffer[160 * 44];    // Screen buffer (7,040 characters)
```
**Critical buffers:**
- `zBuffer` stores depth (1/z) for each screen position
- `buffer` stores the ASCII character to display
- Both are 1D arrays representing 2D screens (index = x + y * width)

```c
int backgroundASCIICode = ' ';  // Space character for transparency
int distanceFromCam = 100;      // Camera distance from origin
float K1 = 40;                  // Perspective projection scaling factor
float incrementSpeed = 0.6;     // Step size when iterating cube surfaces
```
Configuration parameters for rendering and animation.

### Rotation Matrix Functions
```c
float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}
```
**What it does:** Computes the X-coordinate after applying 3D rotation
**Mathematical equivalent:** First row of the combined rotation matrix R = Rz(C) × Ry(B) × Rx(A) multiplied by point (i, j, k)

Breaking down the terms:
- `i * cos(B) * cos(C)` - Contribution from X-axis (i component)
- `j * (sin(A) * sin(B) * cos(C) + cos(A) * sin(C))` - Contribution from Y-axis (j component)
- `k * (sin(A) * sin(C) - cos(A) * sin(B) * cos(C))` - Contribution from Z-axis (k component)

```c
float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}
```
**What it does:** Computes the Y-coordinate after rotation
**Represents:** Second row of the rotation matrix

```c
float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}
```
**What it does:** Computes the Z-coordinate after rotation
**Represents:** Third row of the rotation matrix
**Note:** This is simpler because certain terms cancel out in the matrix multiplication

### Surface Calculation and Rendering
```c
void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
```
**Purpose:** Projects a 3D point onto the 2D screen with depth testing

```c
  x = calculateX(cubeX, cubeY, cubeZ);
  y = calculateY(cubeX, cubeY, cubeZ);
  z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;
```
**Step 1:** Apply rotation matrix to get rotated coordinates
- `distanceFromCam` moves the cube away from camera (prevents z=0)

```c
  ooz = 1 / z;  // "One over Z"
```
**Step 2:** Calculate inverse depth
- Used for both perspective projection and depth testing
- Closer objects have HIGHER ooz values (since z is smaller)

```c
  xp = (int)(width / 2 + K1 * ooz * x * 2);
  yp = (int)(height / 2 + K1 * ooz * y);
```
**Step 3:** Perspective projection
- Dividing by z creates perspective (distant objects appear smaller)
- `width/2` and `height/2` center the image
- `K1` is a scaling factor (field of view control)
- `* 2` on x compensates for character aspect ratio (characters are taller than wide)

```c
  idx = xp + yp * width;
```
**Step 4:** Convert 2D coordinates to 1D array index
- Formula for row-major ordering: index = x + y × width

```c
  if (idx >= 0 && idx < width * height) {
```
**Step 5:** Bounds checking
- Prevents writing outside array bounds (avoiding crashes)

```c
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}
```
**Step 6:** Depth test (Z-buffering)
- Only draw if this point is closer than what's already there
- Updates depth buffer and screen buffer together
- **This creates the 3D occlusion effect** - front faces hide back faces

### Main Function
```c
int main() {
  printf("\x1b[2J");  // ANSI escape code: clear entire screen
```
**Initial screen clear** - prepares terminal for animation

```c
  while (1) {  // Infinite loop - runs until Ctrl+C
```
**Animation loop** - each iteration is one frame

```c
    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0, width * height * 4);
```
**Frame initialization:**
- `memset(buffer, ' ', 7040)` - Fill screen with spaces
- `memset(zBuffer, 0, 28160)` - Reset all depths to 0 (far away)
- Note: `width * height * 4` because each float is 4 bytes

```c
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
```
**Surface generation** - nested loops create a grid of points on each cube face

```c
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');  // Front face
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');    // Right face
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');  // Left face
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');   // Back face
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');  // Bottom face
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');    // Top face
```
**Six cube faces:**
- Each face is a plane where one coordinate is constant (±cubeWidth)
- Different characters help distinguish faces visually
- Front face: z = -20 (all points have cubeZ = -cubeWidth)
- Right face: x = 20 (all points have cubeX = cubeWidth)
- etc.

```c
    printf("\x1b[H");  // ANSI escape: move cursor to home (top-left)
```
**Cursor reset** - positions for next frame without scrolling

```c
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }
```
**Screen output:**
- `putchar(buffer[k])` - Output character at position k
- `k % width ? ... : 10` - If at end of row (k divisible by width), output newline (ASCII 10) instead
- This creates 44 rows of 160 characters each

```c
    A += 0.05;
    B += 0.05;
    C += 0.01;
```
**Angle updates** - incrementing creates continuous rotation
- A and B rotate faster (0.05 rad/frame)
- C rotates slower (0.01 rad/frame)
- Different speeds create interesting tumbling motion

```c
    usleep(8000 * 2);  // Sleep 16,000 microseconds = 16ms
```
**Frame rate control:**
- 16ms per frame ≈ 62.5 FPS
- Prevents excessive CPU usage
- Creates smooth animation

---

## 2. Build Systems: Make vs CMake

### What is a Build System?
A build system automates the compilation process. Instead of typing long gcc commands, you use simple commands like `make`.

### Option 1: Direct Compilation (No Build System)
```bash
gcc cube.c -o cube -lm
./cube
```
**Flags explained:**
- `-o cube` - Output file named "cube"
- `-lm` - Link math library (needed for sin, cos functions)

### Option 2: Makefile
```makefile
.PHONY: clean run
.SILENT:

cube.o: cube.c
	gcc -o $@ $< -lm

run: cube.o
	./$

clean:
	rm -rf cube.o
```
**What each part does:**
- `.PHONY: clean run` - Declares these aren't file targets (prevents conflicts)
- `.SILENT:` - Suppresses command echoing
- `cube.o: cube.c` - Target depends on source file
- `$@` - Expands to target name (cube.o)
- `$<` - Expands to first dependency (cube.c)
- `run:` - Target to compile and execute
- `clean:` - Target to remove compiled files

**Usage:**
```bash
make        # Compiles to cube.o
make run    # Compiles and runs
make clean  # Removes executable
```

**Advantages:**
- Simple for small projects
- Fast to write and understand
- Only rebuilds if source changed (dependency tracking)
- Common convention (developers expect it)

**Disadvantages:**
- Platform-specific (this Makefile is Unix/Linux only)
- Manual updates needed for complex projects
- Doesn't handle cross-platform differences automatically

### Option 3: CMake
```cmake
cmake_minimum_required(VERSION 3.0.0)
project(cube VERSION 0.1.0 LANGUAGES C)

add_executable(cube cube.c)

if (UNIX)
  target_link_libraries(cube m)
endif (UNIX)
```
**What each line does:**
- `cmake_minimum_required` - Ensures CMake version compatibility
- `project(cube ...)` - Names the project, sets version and language
- `add_executable(cube cube.c)` - Creates executable from source
- `if (UNIX) ... endif` - Conditionally links math library (only needed on Unix)

**Usage:**
```bash
mkdir build       # Create separate build directory
cd build
cmake ..          # Generate Makefiles from CMakeLists.txt
make              # Compile using generated Makefile
./cube            # Run
```
**To clean:**
```bash
cd ..
rm -rf build      # Delete entire build directory
```

**Advantages:**
- **Cross-platform** - Works on Windows, Linux, Mac
- Handles platform differences automatically (like -lm flag)
- Generates native build files (Makefiles on Unix, Visual Studio projects on Windows)
- Scales well to large projects
- Industry standard for C/C++ projects
- Out-of-source builds keep source directory clean

**Disadvantages:**
- More complex for simple projects
- Requires learning CMake syntax
- Two-step process (cmake, then make)

### Which Should You Use?

**For this single-file project:**
- **Just use gcc directly** - Simplest and fastest
- **Or keep Makefile** - If you want convenient commands

**For larger projects:**
- **Use CMake** - When you have multiple files, dependencies, or need cross-platform support

**Key Rule:** Don't keep both Makefile and CMakeLists.txt - CMake generates its own Makefile and will overwrite yours!

---

## 3. Memory Safety Analysis

### Why This Program Avoids Memory Errors

#### Fixed-Size Static Arrays
```c
float zBuffer[160 * 44];  // 7,040 floats = 28,160 bytes
char buffer[160 * 44];     // 7,040 chars = 7,040 bytes
```
**Memory allocation:**
- Arrays allocated at compile time (on stack or in data segment)
- Size is constant and known at compile time
- **No malloc/free** - eliminates possibility of memory leaks
- **No dynamic growth** - prevents unbounded memory consumption

#### Memory Reuse Pattern
```c
while (1) {
  memset(buffer, backgroundASCIICode, width * height);
  memset(zBuffer, 0, width * height * 4);
  // ... render frame ...
}
```
**Each frame:**
- Same buffers are **overwritten**, not expanded
- `memset` writes over existing data
- Memory footprint remains constant

#### Bounds Checking
```c
idx = xp + yp * width;
if (idx >= 0 && idx < width * height) {
  // Only write if within bounds
}
```
**Protection against buffer overflow:**
- Validates index before array access
- Prevents writing outside allocated memory
- Guards against projection errors (when points project outside screen)

#### No Dynamic Allocation
**The program never uses:**
- `malloc()` - No heap allocation
- `calloc()` - No cleared allocation
- `realloc()` - No resizing
- `free()` - Nothing to deallocate

**This eliminates:**
- Memory leaks (forgetting to free)
- Double-free errors
- Use-after-free vulnerabilities
- Fragmentation issues

### Memory Profile

**Total static memory:**
```
zBuffer:  28,160 bytes (7,040 floats × 4 bytes)
buffer:    7,040 bytes (7,040 chars × 1 byte)
globals:     ~48 bytes (angles, dimensions, etc.)
-------------------------------------------------
Total:    ~35 KB (constant)
```

**CPU Usage:**
- Calculations per frame: ~1,200 points × 6 faces = 7,200 calculations
- Sleep 16ms between frames
- **Very light CPU load** - ancient computers could run this

**Can run indefinitely:**
- ✓ Memory stays constant
- ✓ No accumulation
- ✓ No leaks
- ✓ Minimal CPU usage

**Only consideration:** Terminal emulator might buffer scrollback history, but the program itself uses `\x1b[H` to reuse the same screen area, minimizing this.

---

## 4. Rotation Matrix Mathematics

### What Rotation Matrices Do

A **rotation matrix** transforms a 3D point around an axis while preserving its distance from the origin.

### Basic 2D Rotation

Before 3D, understand 2D rotation by angle θ:
```
R(θ) = [ cos(θ)  -sin(θ) ]
       [ sin(θ)   cos(θ) ]
```

**Applying to point (x, y):**
```
[ x' ]   [ cos(θ)  -sin(θ) ] [ x ]
[ y' ] = [ sin(θ)   cos(θ) ] [ y ]

x' = x·cos(θ) - y·sin(θ)
y' = x·sin(θ) + y·cos(θ)
```

**Example:** Rotate point (1, 0) by 90°:
```
cos(90°) = 0, sin(90°) = 1

x' = 1·0 - 0·1 = 0
y' = 1·1 + 0·0 = 1

Result: (0, 1) ✓ (correct - rotated counterclockwise)
```

### 3D Rotation Matrices

In 3D, we can rotate around each axis independently:

**Rotation around X-axis by angle A:**
```
Rx(A) = [ 1      0         0      ]
        [ 0   cos(A)   -sin(A)    ]
        [ 0   sin(A)    cos(A)    ]
```
This rotates in the YZ-plane, leaving X unchanged.

**Rotation around Y-axis by angle B:**
```
Ry(B) = [ cos(B)   0   sin(B) ]
        [   0      1     0     ]
        [-sin(B)   0   cos(B)  ]
```
This rotates in the XZ-plane, leaving Y unchanged.

**Rotation around Z-axis by angle C:**
```
Rz(C) = [ cos(C)  -sin(C)   0 ]
        [ sin(C)   cos(C)   0 ]
        [   0        0      1 ]
```
This rotates in the XY-plane, leaving Z unchanged.

### Combined Rotation: The Program's Approach

The program applies rotations in order: **X → Y → Z**

**Combined matrix: R = Rz(C) × Ry(B) × Rx(A)**

Multiplying these three matrices gives:
```
R = [ cos(B)cos(C)
      sin(A)sin(B)cos(C) - cos(A)sin(C)
      cos(A)sin(B)cos(C) + sin(A)sin(C)    ]

    [ cos(B)sin(C)
      sin(A)sin(B)sin(C) + cos(A)cos(C)
      cos(A)sin(B)sin(C) - sin(A)cos(C)    ]

    [-sin(B)
      sin(A)cos(B)
      cos(A)cos(B)                         ]
```

### How the Code Implements This

**First row (X-coordinate):**
```c
float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}
```
This computes: **R[0][0]·i + R[0][1]·j + R[0][2]·k**

Breaking it down:
```
i * cos(B) * cos(C)                                    // i component
+ j * (sin(A) * sin(B) * cos(C) + cos(A) * sin(C))    // j component
+ k * (sin(A) * sin(C) - cos(A) * sin(B) * cos(C))    // k component
```

**Second row (Y-coordinate):**
```c
float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}
```
This computes: **R[1][0]·i + R[1][1]·j + R[1][2]·k**

**Third row (Z-coordinate):**
```c
float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}
```
This computes: **R[2][0]·i + R[2][1]·j + R[2][2]·k**

### Properties of Rotation Matrices

**Orthogonal:** R^T = R^(-1)
- Transpose equals inverse
- Preserves distances and angles

**Determinant = 1:**
- Volume-preserving
- Distinguishes proper rotations from reflections

**Non-commutative (for n > 2):**
- Order matters! Rx(A) × Ry(B) ≠ Ry(B) × Rx(A)
- That's why the cube tumbles differently with different angle orders

### Visualization of What's Happening

**Frame 1 (A=0, B=0, C=0):**
```
Identity matrix = [ 1  0  0 ]
                  [ 0  1  0 ]
                  [ 0  0  1 ]
Point (20, 10, -20) → (20, 10, -20) (unchanged)
```

**Frame 60 (A=3.0, B=3.0, C=0.6):**
```
Complex rotation matrix (calculated from sin/cos of angles)
Point (20, 10, -20) → (rotated x, rotated y, rotated z)
```

**Each frame:**
- All cube vertices are rotated by the current angles
- Projected onto 2D screen
- Z-buffer determines which surfaces are visible
- Creates smooth spinning animation

### Why Use Matrices?

**Alternatives to matrices:**
1. Euler angles directly (gimbal lock problems)
2. Quaternions (more efficient, no gimbal lock)
3. Axis-angle representation

**Matrices chosen because:**
- Direct mathematical formulation
- Easy to understand and verify
- Efficient for single-object rendering
- Educational value (clear connection to linear algebra)

---

## Summary

### The Complete Process
1. **Initialization** - Clear screen, set up buffers
2. **Each frame:**
   - Reset buffers (memset)
   - For each point on cube surface:
     - Apply rotation matrix (calculateX, calculateY, calculateZ)
     - Project to 2D screen (perspective division)
     - Depth test and draw (z-buffer comparison)
   - Output buffer to terminal
   - Increment angles
   - Sleep 16ms
3. **Repeat forever** (until Ctrl+C)

### Key Concepts
- **Rotation matrices** - Mathematical transformation preserving shape
- **Z-buffering** - Depth testing for 3D occlusion
- **Perspective projection** - Dividing by Z for realistic depth
- **Fixed memory** - No dynamic allocation, constant footprint
- **Build systems** - Automation tools (Make simple, CMake cross-platform)

The beauty of this program is its simplicity - pure mathematics rendered as text, with no graphics libraries needed!