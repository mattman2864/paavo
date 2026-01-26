cmake_minimum_required(VERSION 3.11)
project(paavo C)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Find raylib package
find_package(raylib REQUIRED)

# If raylib is not found automatically, you can specify the path
set(raylib_DIR ~/libs/raylib/build)
find_package(raylib REQUIRED)

# Add your executable (replace 'main.c' with your source files)
add_executable(${PROJECT_NAME} 
  src/main.c
  src/player.c
  src/body.c
)

# Link raylib to your executable
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to build directory (optional, if you have a resources folder)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    # Common flags for GCC and Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra -Wpedantic -Werror
    >
    # Common flags for MSVC
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /WX
    >
)
