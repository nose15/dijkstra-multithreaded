//
// Created by lukasz on 25.11.25.
//

#include <dijkstra_algorithm/dijkstra.hpp>
#include <data_structures/concurrent_kqueue.hpp>
#include <atomic>
#include <thread>
#include <limits>
#include <iostream>

std::vector<double> dijkstra(const Graph& g, int source) {
  using PQItem = std::pair<double, int>; // (distance, node)
  const int n = g.size();

  const double INF = std::numeric_limits<double>::infinity();
  std::vector<double> dist(n, INF);
  dist[source] = 0.0;

  // Min-heap by distance
  std::priority_queue<PQItem, std::vector<PQItem>, std::greater<>> pq;
  pq.emplace(0.0, source);

  while (!pq.empty()) {
    auto [d_u, u] = pq.top();
    pq.pop();

    // Skip stale entries
    if (d_u > dist[u])
      continue;

    // Relax edges
    for (const auto& e : g.adj()[u]) {
      int v = e.to;
      double nd = d_u + e.weight;

      if (nd < dist[v]) {
        dist[v] = nd;
        pq.emplace(nd, v);
      }
    }
  }

  return dist;
}

std::vector<double> parallel_dijkstra(const Graph& g, int source, int num_threads, int partitions) {
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

  auto worker = [&](int tid) {
    active_workers.fetch_add(1, std::memory_order_relaxed);

    while (!stop_flag.load(std::memory_order_acquire)) {

      auto opt = queue.pop();
      if (!opt.has_value()) {
        if (queue.empty())
          break;

        std::this_thread::yield();
        continue;
      }

      auto [d_u, u] = *opt;

      // Bounds check to avoid UB if queue is corrupted
      if (u < 0 || u >= (int)adj.size())
        continue;

      double cur = dist_atomic[u].load(std::memory_order_acquire);
      if (d_u > cur)
        continue;

      for (const auto &e : adj[u]) {
        int v = e.to;

        if (v < 0 || v >= (int)dist_atomic.size())
          continue;

        double nd = d_u + e.weight;
        double prev = dist_atomic[v].load(std::memory_order_acquire);

        // CAS loop
        while (nd < prev) {
          if (dist_atomic[v].compare_exchange_weak(
              prev, nd,
              std::memory_order_acq_rel,
              std::memory_order_acquire))
          {
            queue.push(nd, v);
            break;
          }
          // prev now updated to latest value
        }
      }
    }

    active_workers.fetch_sub(1, std::memory_order_relaxed);
  };


  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (int t = 0; t < num_threads; ++t) threads.emplace_back(worker, t);
  for (auto &th : threads) th.join();


// gather results
  std::vector<double> result(n);
  for (int i = 0; i < n; ++i) result[i] = dist_atomic[i].load();
  return result;
}