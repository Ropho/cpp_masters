cmake_minimum_required(VERSION 3.16)
project(13.03_remove_empty_lines LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(13.03 13.15.cpp)
target_compile_options(13.03 PRIVATE -Wall -Wextra -Wpedantic)

configure_file(source.cpp source.cpp COPYONLY)
