cmake_minimum_required(VERSION 3.30)
project(dijkstra_multithreaded_src)

add_subdirectory(dijkstra_algorithm)
add_subdirectory(data_structures)
add_subdirectory(graph_generator)

add_executable(dijkstra_multithreaded_src
        main.cpp
)

target_link_libraries(dijkstra_multithreaded_src
        data_structures
        dijkstra_algorithm
        graph_generator
)

