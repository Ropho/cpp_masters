cmake_minimum_required(VERSION 3.16)
project(12.01_money_locale LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(12.01 12.01.cpp)
target_compile_options(12.01 PRIVATE -Wall -Wextra -Wpedantic)
