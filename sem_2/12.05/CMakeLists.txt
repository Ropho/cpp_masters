cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0167)
	cmake_policy(SET CMP0167 NEW)
endif()

project(12.05_calculators LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(12.19 12.19.cpp)
target_compile_options(12.19 PRIVATE -Wall -Wextra -Wpedantic)

add_executable(12.26 12.26.cpp)
target_include_directories(12.26 SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_options(12.26 PRIVATE -Wall -Wextra -Wpedantic)
