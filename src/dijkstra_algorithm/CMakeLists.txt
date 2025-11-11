cmake_minimum_required(VERSION 3.30)
project(dijkstra_algorithm)

add_library(dijkstra_algorithm)

target_sources(dijkstra_algorithm
    PUBLIC
        include/dijkstra_algorithm/dijkstra.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.cpp
)

target_include_directories(dijkstra_algorithm
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

