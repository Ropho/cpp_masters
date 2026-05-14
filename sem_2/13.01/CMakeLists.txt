cmake_minimum_required(VERSION 3.16)
project(13.01_hex_bytes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(13.01 13.01.cpp)
target_compile_options(13.01 PRIVATE -Wall -Wextra -Wpedantic)
