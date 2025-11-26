cmake_minimum_required(VERSION 3.22)
project(dijkstra_algorithm)

add_library(dijkstra_algorithm)

target_sources(dijkstra_algorithm
    PUBLIC
        include/dijkstra_algorithm/dijkstra.hpp
    PRIVATE
        src/dijkstra.cpp
)

target_include_directories(dijkstra_algorithm
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(dijkstra_algorithm
    data_structures
)
