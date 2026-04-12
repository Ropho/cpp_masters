cmake_minimum_required(VERSION 3.16)
project(10.05_hash_collisions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.65 REQUIRED)

add_executable(10.05 10.05.cpp)
target_include_directories(10.05 PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_options(10.05 PRIVATE -O3)

option(ENABLE_HASH_M32 "Build 32-bit binary (-m32) for Partow-style 32-bit unsigned semantics" OFF)

if(ENABLE_HASH_M32)
	if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		message(WARNING "ENABLE_HASH_M32: -m32 is often unsupported on Apple Silicon; disable on ARM Macs.")
	endif()
	target_compile_options(10.05 PRIVATE -m32)
	target_link_options(10.05 PRIVATE -m32)
endif()
