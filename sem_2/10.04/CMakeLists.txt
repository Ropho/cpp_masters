cmake_minimum_required(VERSION 3.16)
project(10.04_fibonacci_matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(10.04 10.04.cpp)
target_include_directories(10.04 PRIVATE ${Boost_INCLUDE_DIRS})
