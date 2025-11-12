//
// Created by lukasz on 12.11.25.
//

#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_GRAPH_GENERATOR_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_GRAPH_GENERATOR_HPP_

#include <dijkstra_algorithm/edge.hpp>

#include <vector>

namespace dijkstra_algorithm {

std::vector<std::vector<Edge>> generateRandomGraph(int n, double density, bool directed = true);

}

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DIJKSTRA_ALGORITHM_GRAPH_GENERATOR_HPP_
