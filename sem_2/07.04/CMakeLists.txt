cmake_minimum_required(VERSION 3.14)
project(07.04_sort_tests LANGUAGES CXX)

################################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

################################################################################

enable_testing()

add_executable(sort_test sort_test.cpp)

target_link_libraries(sort_test PRIVATE GTest::gtest)

target_include_directories(sort_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################

include(GoogleTest)
gtest_discover_tests(sort_test)

################################################################################
