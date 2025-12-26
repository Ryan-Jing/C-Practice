# C-Practice
Practicing C

I want to make small projects to practice C passively. I realized didn't even know how to compile C without an IDE, and I'd like to learn more.

I also want to practice writing ledgible code that requires very few in-line comments, and only
include Doxygen style function comments to maintain common industry practice. I want to see if
very clearly named variables that may be long in length are more advantageous.

# C Compilation Guide

A concise reference for compiling C programs using GCC, Clang, Make, and CMake.

---

## Table of Contents
1. [The Compilation Process](#the-compilation-process)
2. [GCC/Clang - Direct Compilation](#gccclang---direct-compilation)
3. [Make - Build Automation](#make---build-automation)
4. [CMake - Cross-Platform Builds](#cmake---cross-platform-builds)
5. [Quick Reference](#quick-reference)

---

## The Compilation Process

C compilation happens in 4 stages:

```
Source (.c) → Preprocessor → Compiler → Assembler → Linker → Executable
   file.c        file.i        file.s      file.o      libraries    program
```

1. **Preprocessing**: Expands `#include`, `#define`, removes comments
2. **Compilation**: Converts C to assembly language
3. **Assembly**: Converts assembly to machine code (object files)
4. **Linking**: Combines object files and libraries into executable

---

## GCC/Clang - Direct Compilation

### Basic Commands

**Compile and run:**
```bash
gcc main.c -o program
./program
```

**With common flags:**
```bash
gcc main.c -o program -Wall -Wextra -g -O2
```

**Multiple files:**
```bash
gcc main.c utils.c network.c -o program
```

**Link external library:**
```bash
gcc main.c -o program -lm -lpthread
```

### Compilation Stages (Step-by-Step)

```bash
# 1. Preprocess only
gcc -E main.c -o main.i

# 2. Compile to assembly
gcc -S main.c -o main.s

# 3. Assemble to object file
gcc -c main.c -o main.o

# 4. Link object files
gcc main.o utils.o -o program
```

### Common Flags

| Flag | Purpose |
|------|---------|
| `-o <name>` | Name the output file |
| `-c` | Compile only (don't link) |
| `-Wall` | Enable all warnings |
| `-Wextra` | Enable extra warnings |
| `-Werror` | Treat warnings as errors |
| `-g` | Include debug symbols (for gdb) |
| `-O0` / `-O1` / `-O2` / `-O3` | Optimization level |
| `-std=c11` | Use C11 standard |
| `-I<dir>` | Add include directory |
| `-L<dir>` | Add library directory |
| `-l<name>` | Link library (e.g., `-lm` for math) |

### Common Libraries

```bash
-lm          # Math library (sin, cos, sqrt, etc.)
-lpthread    # POSIX threads
-lssl        # OpenSSL
-lcurl       # libcurl
```

---

## Make - Build Automation

### Basic Makefile

```makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

# Target executable
TARGET = program

# Source files
SRCS = main.c utils.c network.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
    $(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Run
run: $(TARGET)
    ./$(TARGET)

# Clean
clean:
    rm -f $(OBJS) $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all run clean rebuild
```

### Using Make

```bash
make              # Build
make run          # Build and run
make clean        # Remove build artifacts
make rebuild      # Clean then build
make -j8          # Parallel build (8 jobs)
```

### Makefile Variables

| Variable | Meaning |
|----------|---------|
| `$@` | Target name |
| `$<` | First dependency |
| `$^` | All dependencies |
| `$?` | Dependencies newer than target |
| `$(CC)` | Compiler variable |
| `$(CFLAGS)` | Compiler flags |

---

## CMake - Cross-Platform Builds

### Basic CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Source files
set(SOURCES
    main.c
    utils.c
    network.c
)

# Create executable
add_executable(program ${SOURCES})

# Link libraries
target_link_libraries(program m pthread)

# Add include directories (optional)
target_include_directories(program PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add compiler flags (optional)
target_compile_options(program PRIVATE -Wall -Wextra)
```

### Using CMake

**Two-step process:**

```bash
# Step 1: Configure (generate build files)
cmake -S . -B build

# Step 2: Build
cmake --build build

# Step 3: Run
./build/program
```

**One-liner:**
```bash
cmake -S . -B build && cmake --build build && ./build/program
```

### CMake Build Types

```bash
# Debug build (with symbols, no optimization)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release build (optimized, no debug info)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Release with debug info
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

### CMake Options

```bash
# Specify compiler
cmake -S . -B build -DCMAKE_C_COMPILER=gcc

# Parallel build
cmake --build build -j8

# Verbose output
cmake --build build --verbose

# Clean
cmake --build build --target clean
# Or just: rm -rf build

# Install
cmake --build build --target install
```

### Advanced CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES C)

# Options
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build type specific flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Warnings
if(ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

# Source files
set(SOURCES
    src/main.c
    src/utils.c
    src/network.c
)

# Create executable
add_executable(program ${SOURCES})

# Link libraries
if(UNIX)
    target_link_libraries(program m pthread)
endif()

# Include directories
target_include_directories(program PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Install
install(TARGETS program DESTINATION bin)

# Testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

---

## Quick Reference

### When to Use Each Tool

| Tool | Best For | Command |
|------|----------|---------|
| **GCC/Clang** | Single files, quick tests, learning | `gcc file.c -o program` |
| **Make** | Small-medium projects (2-20 files) | `make` |
| **CMake** | Large projects, cross-platform, teams | `cmake -S . -B build && cmake --build build` |

### Common Tasks

**Compile single file:**
```bash
gcc main.c -o program
```

**Compile with debugging:**
```bash
gcc main.c -o program -g -Wall
```

**Compile optimized:**
```bash
gcc main.c -o program -O3
```

**Compile multiple files:**
```bash
gcc main.c utils.c -o program
```

**Link math library:**
```bash
gcc main.c -o program -lm
```

**Create object files separately:**
```bash
gcc -c main.c -o main.o
gcc -c utils.c -o utils.o
gcc main.o utils.o -o program
```

**Using Make:**
```bash
make              # Build
make clean        # Clean
make -j8          # Parallel build
```

**Using CMake:**
```bash
cmake -S . -B build                           # Configure
cmake --build build                           # Build
cmake --build build -j8                       # Parallel build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug # Debug build
```

### File Extensions

```
.c       C source code
.h       C header file
.o       Object file (compiled, not linked)
.a       Static library
.so      Shared library (Linux)
.dylib   Shared library (macOS)
.dll     Shared library (Windows)
```

### Troubleshooting

**Undefined reference to function:**
- Missing source file or library
- Add `-l<library>` flag
- Example: `undefined reference to 'sqrt'` → add `-lm`

**Header file not found:**
- Add include directory with `-I<path>`
- Example: `gcc main.c -I/usr/local/include`

**Library not found:**
- Add library directory with `-L<path>`
- Example: `gcc main.c -L/usr/local/lib -lmylib`

**Permission denied:**
```bash
chmod +x program
./program
```

---

## Project Structure Examples

### Simple Project (GCC)
```
project/
├── main.c
├── utils.c
├── utils.h
└── compile.sh
```

```bash
#!/bin/bash
gcc main.c utils.c -o program -Wall -g
```

### Medium Project (Make)
```
project/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── network.c
├── include/
│   ├── utils.h
│   └── network.h
├── Makefile
└── README.md
```

### Large Project (CMake)
```
project/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── network.c
├── include/
│   ├── utils.h
│   └── network.h
├── tests/
│   └── test_utils.c
├── build/          (generated)
├── CMakeLists.txt
└── README.md
```

---

## Summary

### The Build Pipeline

```
Source Code (.c)
    ↓
GCC/Clang (compiler)
    ↓
Object Files (.o)
    ↓
Linker
    ↓
Executable
```

### With Build Tools

```
Source Code (.c)
    ↓
Make → GCC → Executable
    ↓
CMake → Makefile → Make → GCC → Executable
```

### Key Takeaways

1. **GCC/Clang**: Direct compilation, full control
2. **Make**: Automates builds, tracks dependencies
3. **CMake**: Generates build files, cross-platform
4. **All roads lead to GCC/Clang**: Make and CMake just automate calling the compiler

### Common Workflow

```bash
# Quick test
gcc test.c -o test && ./test

# Small project
make && ./program

# Large project
cmake -S . -B build && cmake --build build && ./build/program
```

---
