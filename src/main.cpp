#include <graph_generator/graph_generator.hpp>
#include <dijkstra_algorithm/dijkstra.hpp>

#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char **argv) {
  std::vector<double> densities{0.1, 0.25, 0.5, 0.75, 0.99};
  std::vector<int> thread_counts{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
  std::vector<int> vertex_counts{1024, 2048, 4096, 8192, 16384, 32768};

  for (auto vertex_count : vertex_counts) {
    for (auto density : densities) {
      std::cout << "Generating for n=" << vertex_count << ", density=" << density << std::endl;
      auto g = generate_random_graph(vertex_count, density);
      std::cout << "Generated for n=" << vertex_count << ", density=" << density << std::endl;
      std::cout << "Running regular dijkstra" << std::endl;

      auto time_start = std::chrono::high_resolution_clock::now();
      // TODO: Run regular dijkstra
      auto time_end = std::chrono::high_resolution_clock::now();
      auto reference_duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);

      std::cout << "Reference time at +" << reference_duration.count() << "us" << std::endl;

      for (auto thread_count : thread_counts) {
        std::cout << "Running for " << thread_count << " threads..." << std::endl;

        time_start = std::chrono::high_resolution_clock::now();
        auto dist = parallel_dijkstra(g, 0, thread_count, thread_count * 2);
        time_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
        std::cout << duration.count() << "us" << std::endl;
      }
    }
  }

  return 0;
}