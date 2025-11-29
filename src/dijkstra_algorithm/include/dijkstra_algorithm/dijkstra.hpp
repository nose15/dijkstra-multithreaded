#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_DIJKSTRA_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_DIJKSTRA_HPP_

#include <data_structures/graph.hpp>
#include <vector>

#include <memory>

std::vector<double> dijkstra(const Graph& g, int source);
std::vector<double> parallel_dijkstra(const Graph& g, int source, int num_threads = 4, int partitions = 8);

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_DIJKSTRA_HPP_
