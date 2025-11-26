#include <data_structures/graph.hpp>
#include <dijkstra_algorithm/dijkstra.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " GRAPH_FILE SOURCE [NUM_THREADS] [PARTITIONS]\n";
    return 1;
  }
  std::string path = argv[1];
  int source = std::stoi(argv[2]);
  int num_threads = (argc >= 4) ? std::stoi(argv[3]) : 4;
  int partitions = (argc >= 5) ? std::stoi(argv[4]) : std::max(4, num_threads*2);


  std::ifstream ifs(path);
  if (!ifs) { std::cerr << "Cannot open " << path << "\n"; return 2; }


  int N, M;
  ifs >> N >> M;
  Graph g(N);
  for (int i = 0; i < M; ++i) {
    int u, v; double w;
    ifs >> u >> v >> w;
    g.add_edge(u, v, w);
  }


  auto dist = parallel_dijkstra(g, source, num_threads, partitions);
  std::cout << std::fixed << std::setprecision(6);
  for (int i = 0; i < (int)dist.size(); ++i) {
    if (dist[i] == std::numeric_limits<double>::infinity()) std::cout << i << ": INF\n";
    else std::cout << i << ": " << dist[i] << "\n";
  }
  return 0;
}