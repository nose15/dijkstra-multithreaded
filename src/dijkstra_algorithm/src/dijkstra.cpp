//
// Created by lukasz on 25.11.25.
//

#include <dijkstra_algorithm/dijkstra.hpp>
#include <data_structures/concurrent_kqueue.hpp>
#include <atomic>
#include <thread>
#include <limits>
#include <iostream>

std::vector<double> parallel_dijkstra(const Graph &g, int source, int num_threads, int partitions) {
  int n = g.size();
  const auto &adj = g.adj();
  const double INF = std::numeric_limits<double>::infinity();


// distances as atomics so threads can read/update concurrently
  std::vector<std::atomic<double>> dist_atomic(n);
  for (int i = 0; i < n; ++i) dist_atomic[i].store(INF);
  dist_atomic[source].store(0.0);


  ConcurrentKQueue queue(partitions, 2);
  queue.push(0.0, source);


  std::atomic<int> active_workers{0};
  std::atomic<bool> stop_flag{false};


  auto worker = [&](int tid){
    ++active_workers;
    while (!stop_flag.load()) {
      auto opt = queue.pop();
      if (!opt.has_value()) {
// no item available; if queue empty and no other workers active, finish
        if (queue.empty()) break;
// otherwise busy-wait briefly
        std::this_thread::yield();
        continue;
      }
      auto [d_u, u] = *opt;
// Check if this is an outdated entry
      double cur = dist_atomic[u].load();
      if (d_u > cur + 1e-15) continue; // stale entry


// relax neighbors
      for (const auto &e : adj[u]) {
        int v = e.to;
        double nd = d_u + e.weight;
        double prev = dist_atomic[v].load();
        while (nd + 1e-15 < prev) {
// attempt to set a better distance
          if (dist_atomic[v].compare_exchange_weak(prev, nd)) {
// pushed new candidate into queue
            queue.push(nd, v);
            break;
          }
// compare_exchange_weak updated prev with current value; loop if still larger
        }
      }
    }
    --active_workers;
  };


  std::vector<std::thread> threads;
  for (int t = 0; t < num_threads; ++t) threads.emplace_back(worker, t);
  for (auto &th : threads) th.join();


// gather results
  std::vector<double> result(n);
  for (int i = 0; i < n; ++i) result[i] = dist_atomic[i].load();
  return result;
}