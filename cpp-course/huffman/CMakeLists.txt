cmake_minimum_required(VERSION 3.5)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/library)

add_library(huffman-library STATIC library/library.cpp library/library.h)

add_executable(huffman-utility
        utility/utility.cpp)

target_link_libraries(huffman-utility huffman-library)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -g -D_GLIBCXX_DEBUG")
endif()