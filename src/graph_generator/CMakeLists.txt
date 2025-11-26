cmake_minimum_required(VERSION 3.22)
project(graph_generator)

add_library(graph_generator)

target_sources(graph_generator
        PUBLIC
        include/graph_generator/graph_generator.hpp
        PRIVATE
        src/graph_generator.cpp
)

target_include_directories(graph_generator
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(graph_generator
        data_structures
)
