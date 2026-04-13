#############################################################################################

cmake_minimum_required(VERSION 3.16)

#############################################################################################

project(06.05_boost_dll LANGUAGES CXX)

#############################################################################################

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

#############################################################################################

find_package(Boost REQUIRED COMPONENTS filesystem)

#############################################################################################

add_library               (library_v1 SHARED library_v1.cpp)

target_link_libraries     (library_v1 PRIVATE Boost::headers)

#############################################################################################

add_library               (library_v2 SHARED library_v2.cpp)

target_link_libraries     (library_v2 PRIVATE Boost::headers)

#############################################################################################

add_executable            (06.17 06.17.cpp)

target_link_libraries     (06.17 PRIVATE Boost::filesystem)

#############################################################################################
