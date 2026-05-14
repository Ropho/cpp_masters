cmake_minimum_required(VERSION 3.16)
project(13.04_directory_regex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(13.04 13.18.cpp)
target_compile_options(13.04 PRIVATE -Wall -Wextra -Wpedantic)
