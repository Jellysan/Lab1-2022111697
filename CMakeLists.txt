cmake_minimum_required(VERSION 3.31)
project(BlackTest)

set(CMAKE_CXX_STANDARD 20)

add_library(corelib
        DirectedGraph.cpp
)

enable_testing()
include(GoogleTest)

add_subdirectory(lib/googletest)

add_executable(BlackTest
        test/test_black.cpp
)

target_include_directories(BlackTest PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest/googletest/include
)

target_link_libraries(BlackTest corelib gtest gtest_main)

gtest_discover_tests(BlackTest)

# 代码覆盖率（可选）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
