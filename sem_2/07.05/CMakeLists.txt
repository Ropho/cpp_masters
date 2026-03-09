cmake_minimum_required(VERSION 3.14)
project(07.05_sort_benchmark LANGUAGES CXX)

################################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

include(FetchContent)

# Google Test (for unit tests)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Google Benchmark
FetchContent_Declare(
	googlebenchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG        v1.8.3
)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

################################################################################

enable_testing()

# Unit tests
add_executable(sort_test sort_test.cpp)
target_link_libraries(sort_test PRIVATE GTest::gtest)
target_include_directories(sort_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(sort_test)

################################################################################

# Microbenchmark (parameterized by insertion-sort threshold)
add_executable(sort_benchmark sort_benchmark.cpp)
target_link_libraries(sort_benchmark PRIVATE benchmark::benchmark)
target_include_directories(sort_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################
