cmake_minimum_required(VERSION 3.16)
project(11.03_quicksort_comparator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(11.03 11.03.cpp)
target_compile_options(11.03 PRIVATE -Wall -Wextra -Wpedantic)
