cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Dino VERSION 0.1 LANGUAGES C)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/background.c
    src/render.c
    src/sprite.c
    src/terminal.c
    src/scenes.c
    src/scenes/scene_zero.c
    src/scenes/scene_one.c
    src/scenes/scene_two.c
    src/scenes/scene_three.c
    src/scenes/scene_four.c
    src/scenes/scene_five.c
    src/scenes/scene_six.c
    src/scenes/scene_seven.c
    src/scenes/scene_eight.c
    mylove.c
)

# Header files (for IDE support)
set(HEADERS
    include/ascii.h
    include/background.h
    include/render.h
    include/scenes.h
    include/sprites.h
    include/terminal.h
    include/textures.h
    include/common.h
)

# Create executable
add_executable(mylove ${SOURCES} ${HEADERS})

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mylove PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-newline-eof           # Ignore missing newline at end of file
        -Wno-strict-prototypes     # Ignore strict prototype warnings (optional)
    )
endif()

# Platform-specific settings
if(UNIX AND NOT APPLE)
    # Linux-specific settings
    target_link_libraries(mylove m)  # Link math library
elseif(APPLE)
    # macOS-specific settings
    target_link_libraries(mylove m)  # Link math library
endif()

# Installation rules
install(TARGETS mylove DESTINATION bin)
# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
