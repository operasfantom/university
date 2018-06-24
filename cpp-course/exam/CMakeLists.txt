cmake_minimum_required(VERSION 3.10)
project(exam)

set(CMAKE_CXX_STANDARD 17)

add_library(lib_set circular_buffer.h circular_buffer.cpp)

add_executable(main main.cpp circular_buffer.cpp)
add_executable(testing
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc
        testing2.cpp
        counted.h
        counted.cpp
        fault_injection.h
        fault_injection.cpp
        )

enable_testing()
target_link_libraries(main lib_set)
target_link_libraries(testing -lpthread)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
        #-fsanitize=address,undefined
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,leak -g -O0 -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")
endif()