//
// Created by lukasz on 25.11.25.
//

#include <graph_generator/graph_generator.hpp>
#include <data_structures/graph.hpp>

#include <random>

Graph generate_random_graph(int n, int m, double max_w) {
  Graph g(n);
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist_v(0, n - 1);
  std::uniform_real_distribution<double> dist_w(1.0, max_w);


  for (int i = 0; i < m; ++i) {
    int u = dist_v(rng);
    int v = dist_v(rng);
    if (u == v) v = (v + 1) % n;
    double w = dist_w(rng);
    g.add_edge(u, v, w);
  }
  return g;
}
