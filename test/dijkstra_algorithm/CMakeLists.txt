cmake_minimum_required(VERSION 3.22)
project(dijkstra_test)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(
        dijkstra_test
        dijkstra_test.cpp
)
target_link_libraries(
        dijkstra_test
        dijkstra_algorithm
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dijkstra_test)

