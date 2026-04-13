cmake_minimum_required(VERSION 3.16)
project(10.05_hash_collisions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Single-configuration default (enables -O3-style opts)" FORCE)
endif()

find_package(Boost 1.65 REQUIRED)

add_executable(10.05 10.05.cpp)
target_include_directories(10.05 PRIVATE ${Boost_INCLUDE_DIRS})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
	target_compile_options(10.05 PRIVATE -O3 -Wall -Wextra -Wpedantic)
elseif(MSVC)
	target_compile_options(10.05 PRIVATE /O2 /W4)
endif()

option(ENABLE_HASH_M32 "Build 32-bit (-m32) as required by the assignment on x86 multilib toolchains" OFF)

if(ENABLE_HASH_M32)
	if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		message(WARNING "ENABLE_HASH_M32: -m32 is often unsupported on Apple Silicon; disable on ARM Macs.")
	endif()
	target_compile_options(10.05 PRIVATE -m32)
	target_link_options(10.05 PRIVATE -m32)
endif()
