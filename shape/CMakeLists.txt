cmake_minimum_required(VERSION 3.10.0)
project(shape VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(shape shape.c)

# Link math library on Unix-like systems
if(UNIX)
    target_link_libraries(shape m)
endif()

# Install the executable
install(TARGETS shape DESTINATION bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)