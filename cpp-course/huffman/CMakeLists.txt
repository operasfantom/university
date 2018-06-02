cmake_minimum_required(VERSION 3.5)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_library(huffman-library STATIC library/huffman_tree.cpp library/huffman_tree.h tests/testing.cpp library/binary_io.cpp library/binary_io.h library/bit_container.cpp library/bit_container.h)

add_executable(huffman-utility
        utility/utility.cpp)

add_executable(huffman-testing
        tests/gtest/gtest-all.cc
        tests/gtest/gtest.h
        tests/gtest/gtest_main.cc
        tests/testing.cpp
        )

target_link_libraries(huffman-utility huffman-library)
target_link_libraries(huffman-testing huffman-library -lgmp -lgmpxx -lpthread)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -g -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g --coverage")
endif()