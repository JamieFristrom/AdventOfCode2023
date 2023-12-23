cmake_minimum_required(VERSION 3.14)
project(aoc-16-test)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    aoc-16-test
    aoc-16-test.cpp
)

target_link_libraries(
    aoc-16-test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(aoc-16-test)


