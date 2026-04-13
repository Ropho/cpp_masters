cmake_minimum_required(VERSION 3.16)
project(10.01_vector_deque LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(10.01 10.01.cpp)
target_compile_options(10.01 PRIVATE -Wall -Wextra -Wpedantic)
