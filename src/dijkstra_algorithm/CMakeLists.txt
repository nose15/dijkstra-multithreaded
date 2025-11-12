cmake_minimum_required(VERSION 3.30)
project(dijkstra_algorithm)

add_library(dijkstra_algorithm)

target_sources(dijkstra_algorithm
    PUBLIC
        include/dijkstra_algorithm/dijkstra.hpp
        include/dijkstra_algorithm/edge.hpp
        include/dijkstra_algorithm/graph_generator.hpp
    PRIVATE
        src/dijkstra.cpp
        src/graph_generator.cpp
)

target_include_directories(dijkstra_algorithm
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

