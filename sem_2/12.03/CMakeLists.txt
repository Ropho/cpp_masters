cmake_minimum_required(VERSION 3.16)
project(12.03_longest_palindrome LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(12.03 12.03.cpp)
target_compile_options(12.03 PRIVATE -Wall -Wextra -Wpedantic)
