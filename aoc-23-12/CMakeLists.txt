cmake_minimum_required(VERSION 3.14)
project(aoc-23-12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googltetest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    aoc_23_12
    aoc_23_12_test.cpp
)

target_link_libraries(
    aoc_23_12_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(aoc_23_12)