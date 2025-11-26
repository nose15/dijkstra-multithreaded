#include <graph_generator/graph_generator.hpp>
#include <dijkstra_algorithm/dijkstra.hpp>

#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char **argv) {
  std::vector<double> densities{0.1, 0.25, 0.5, 0.75, 0.99};
  std::vector<int> thread_counts{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
  std::vector<int> vertex_counts{1024, 2048, 4096, 8192, 16384, 32768};

  std::cout << "vertex_count;density;thread_count;reference_duration;duration" << std::endl;

  for (auto vertex_count : vertex_counts) {
    for (auto density : densities) {
      auto g = generate_random_graph(vertex_count, density);

      auto time_start = std::chrono::high_resolution_clock::now();
      auto dist_ref = dijkstra(g, 0);
      auto time_end = std::chrono::high_resolution_clock::now();
      auto reference_duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);

      for (auto thread_count : thread_counts) {
        time_start = std::chrono::high_resolution_clock::now();
        auto dist = parallel_dijkstra(g, 0, thread_count, thread_count * 2);
        time_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
        std::cout << vertex_count << ";" << density << ";" << thread_count << ";" << reference_duration.count() << ";" << duration.count() << std::endl;
      }
    }
  }

  return 0;
}