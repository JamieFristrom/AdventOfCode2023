cmake_minimum_required(VERSION 3.14)

project(aoc-23-12)

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
    aoc-23-12
    aoc-12-test.cpp
)

target_compile_options(aoc-23-12 PRIVATE /W4 /WX)

target_link_libraries(
    aoc-23-12
    GTest::gtest_main
)

target_include_directories(
    aoc-23-12
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)

enable_testing()