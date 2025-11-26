//
// Created by lukasz on 25.11.25.
//

#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_

#include <vector>
#include <queue>
#include <mutex>
#include <optional>
#include <random>
#include <limits>

class ConcurrentKQueue {
 public:
  using PQKey = double;
  using PQVal = int;
  using PQItem = std::pair<PQKey, PQVal>; // (distance, vertex)

  explicit ConcurrentKQueue(int partitions, int sample_k);

  void push(PQKey key, PQVal val);
  std::optional<PQItem> pop();
  bool empty() const;

 private:
  struct LocalPQ {
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<>> pq;
    mutable std::mutex mtx;
  };

  int k;          // number of partitions
  int sample_k;   // how many partitions to sample on pop()
  std::vector<LocalPQ> locals;

  // No shared RNG — unsafe. Workers must call thread_local RNG inside pop().
};

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_
