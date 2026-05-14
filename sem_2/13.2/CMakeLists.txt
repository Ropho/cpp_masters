cmake_minimum_required(VERSION 3.16)
project(13.2_file_calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(13.2 13.2.cpp)
target_compile_options(13.2 PRIVATE -Wall -Wextra -Wpedantic)

configure_file(tests.txt tests.txt COPYONLY)
