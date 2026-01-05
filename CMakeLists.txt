cmake_minimum_required(VERSION 3.15)
project(paavo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(paavo src/main.cpp)
target_link_libraries(paavo raylib)
