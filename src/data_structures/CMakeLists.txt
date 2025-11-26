cmake_minimum_required(VERSION 3.30)
project(data_structures)

add_library(data_structures)

target_sources(data_structures
        PUBLIC
        include/data_structures/concurrent_kqueue.hpp
        include/data_structures/graph.hpp
        PRIVATE
        src/concurrent_kqueue.cpp
        src/graph.cpp
)

target_include_directories(data_structures
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

