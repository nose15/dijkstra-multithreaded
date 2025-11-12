//
// Created by lukasz on 11.11.25.
//

#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_INCLUDE_DIJKSTRA_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_INCLUDE_DIJKSTRA_HPP_

#include <dijkstra_algorithm/edge.hpp>

#include <vector>

namespace dijkstra_algorithm {

std::vector<int> dijkstra(int n, int start, const std::vector<std::vector<Edge>>& graph);

}

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_INCLUDE_DIJKSTRA_HPP_
