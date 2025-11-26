//
// Created by lukasz on 25.11.25.
//

#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_GRAPH_GRAPH_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_GRAPH_GRAPH_HPP_

#include <vector>
#include <utility>

struct Edge { int to; double weight; };


class Graph {
 public:
  Graph() = default;
  explicit Graph(int n);
  int size() const;
  void add_edge(int u, int v, double w);
  const std::vector<std::vector<Edge>>& adj() const;
 private:
  std::vector<std::vector<Edge>> m_adj;
};

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_GRAPH_GRAPH_HPP_
