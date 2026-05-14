cmake_minimum_required(VERSION 3.16)
project(11.06_travelling_salesman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(11.06 11.06.cpp)
target_include_directories(11.06 SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_options(11.06 PRIVATE -Wall -Wextra -Wpedantic)
