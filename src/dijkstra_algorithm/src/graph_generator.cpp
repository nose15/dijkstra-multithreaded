//
// Created by lukasz on 12.11.25.
//

#include <dijkstra_algorithm/graph_generator.hpp>

#include <vector>
#include <random>
#include <algorithm>

namespace dijkstra_algorithm {

std::vector<std::vector<Edge>> generateRandomGraph(int n, double density, bool directed) {
  std::vector<std::vector<Edge>> graph(n);
  std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
  std::uniform_real_distribution<double> probDist(0.0, 1.0);
  std::uniform_int_distribution<int> weightDist(1, 20);

  // STEP 1: Build a random spanning tree to ensure connectivity
  std::vector<int> vertices(n);
  iota(vertices.begin(), vertices.end(), 0);
  std::shuffle(vertices.begin(), vertices.end(), rng);

  for (int i = 1; i < n; ++i) {
    int u = vertices[i];
    int v = vertices[rng() % i]; // connect to a previous vertex
    int w = weightDist(rng);
    graph[u].push_back({v, w});
    if (!directed)
      graph[v].push_back({u, w});
  }

  // STEP 2: Add extra random edges based on density
  int maxEdges = directed ? n * (n - 1) : n * (n - 1) / 2;
  int currentEdges = directed ? 0 : 0;
  for (int u = 0; u < n; ++u)
    currentEdges += graph[u].size();

  int targetEdges = static_cast<int>(maxEdges * density);
  std::uniform_int_distribution<int> vertexDist(0, n - 1);

  while (currentEdges < targetEdges) {
    int u = vertexDist(rng);
    int v = vertexDist(rng);
    if (u == v) continue; // skip self-loops

    bool exists = false;
    for (auto& e : graph[u]) {
      if (e.to == v) {
        exists = true;
        break;
      }
    }
    if (exists) continue;

    int w = weightDist(rng);
    graph[u].push_back({v, w});
    if (!directed)
      graph[v].push_back({u, w});

    currentEdges++;
    if (!directed) currentEdges++; // count both directions
  }

  return graph;
}

}