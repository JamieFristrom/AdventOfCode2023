cmake_minimum_required(VERSION 3.14)

project(aoc-23-24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest    
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
    googletest
)

add_executable(
    aoc-23-24
    aoc-24-test.cpp
)

target_link_libraries(
    aoc-23-24
    GTest::gtest_main
)

enable_testing()