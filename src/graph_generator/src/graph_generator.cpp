#include <graph_generator/graph_generator.hpp>
#include <data_structures/graph.hpp>

#include <random>

Graph generate_random_graph(int n, double density, double max_w) {
  Graph g(n);
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> prob(0.0, 1.0);
  std::uniform_real_distribution<double> wdist(1.0, max_w);


  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (u == v) continue;
      if (prob(rng) < density) {
        g.add_edge(u, v, wdist(rng));
      }
    }
  }
  return g;
}
