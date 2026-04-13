cmake_minimum_required(VERSION 3.14)
project(09_11_pmr_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
	googlebenchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG        v1.8.3
)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(09.11 09.11.cpp)
target_link_libraries(09.11 PRIVATE benchmark::benchmark)
target_compile_options(09.11 PRIVATE -Wall -Wextra -Wpedantic)
