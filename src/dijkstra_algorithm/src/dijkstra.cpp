//
// Created by lukasz on 11.11.25.
//

#include <dijkstra_algorithm/dijkstra.hpp>

#include <vector>
#include <queue>
#include <limits>

namespace dijkstra_algorithm {

const int INF = std::numeric_limits<int>::max();

void dijkstra(int n, int start, const std::vector<std::vector<Edge>> &graph) {
  std::vector<int> dist(n, INF);
  dist[start] = 0;

  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
  pq.emplace(0, start);

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (d > dist[u]) continue;

    for (const auto& edge : graph[u]) {
      int v = edge.to;
      int w = edge.weight;

      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        pq.emplace(dist[v], v);
      }
    }
  }
}

}

