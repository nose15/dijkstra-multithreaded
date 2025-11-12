#include <dijkstra_algorithm/dijkstra.hpp>
#include <dijkstra_algorithm/graph_generator.hpp>

#include <iostream>

int main() {
  auto graph = dijkstra_algorithm::generateRandomGraph(10, 0.25);
  auto res = dijkstra_algorithm::dijkstra(10, 0, graph);

  for (int i = 0; i < graph.size(); i++) {
    const auto& edge = graph.at(i);
    std::cout << i << ". -> ";

    for (auto v : edge) {
      std::cout << "(to: " << v.to << ", weight: " << v.weight << "), ";
    }

    std::cout << std::endl;
  }

  for (int re : res) {
    std::cout << re << ", ";
  }
  std::cout << std::endl;

  return 0;
}
