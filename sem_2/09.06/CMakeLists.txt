cmake_minimum_required(VERSION 3.16)
project(09.06_fibonacci_iterator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(09.06 09.06.cpp)
target_include_directories(09.06 PRIVATE ${Boost_INCLUDE_DIRS})
