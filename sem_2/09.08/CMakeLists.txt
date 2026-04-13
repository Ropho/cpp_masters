cmake_minimum_required(VERSION 3.14)
project(09_08_entity_operator_new LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(09.08 09.08.cpp)
target_compile_options(09.08 PRIVATE -Wall -Wextra -Wpedantic)
