cmake_minimum_required(VERSION 3.14)
project(09_10_polymorphic_allocators LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(09.10 09.10.cpp)
target_compile_options(09.10 PRIVATE -Wall -Wextra -Wpedantic)
