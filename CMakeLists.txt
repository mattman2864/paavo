cmake_minimum_required(VERSION 3.15)
project(paavo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(raylib_DIR "$ENV{HOME}/libs/raylib/install/lib/cmake/raylib")
find_package(raylib REQUIRED)

add_executable(paavo
    src/main.cpp
    src/Body.cpp
    src/Sprite.cpp
    src/Input.cpp
    src/Player.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(paavo raylib)
