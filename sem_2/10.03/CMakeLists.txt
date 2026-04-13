cmake_minimum_required(VERSION 3.16)
project(10.03_game_of_life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(10.03 10.03.cpp)
target_include_directories(10.03 PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_options(10.03 PRIVATE -Wall -Wextra -Wpedantic -Wno-deprecated-declarations)
