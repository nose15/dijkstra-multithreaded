//
// Created by lukasz on 25.11.25.
//

#ifndef DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_
#define DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_

#include <vector>
#include <queue>
#include <mutex>
#include <random>
#include <optional>
#include <utility>

// A simple k-way partitioned priority queue for parallel Dijkstra.
// Each partition has its own mutex and heap. To pop an item that is
// near-global-min, a thread samples a small set of partitions and
// chooses the smallest among their tops (multi-queue technique).

using PQKey = double; // distance
using PQVal = int; // vertex id
using PQItem = std::pair<PQKey, PQVal>;


class ConcurrentKQueue {
 public:
// partitions: number of local queues
// sample_k: how many queues to sample when popping
  ConcurrentKQueue(int partitions, int sample_k = 2);


// push item into queue assigned by hashing 'val'
  void push(PQKey key, PQVal val);


// try to pop a near-min item by sampling sample_k queues; returns nullopt if all empty
  std::optional<PQItem> pop();


// return whether all queues are empty
  bool empty() const;


 private:
  struct LocalQueue {
    mutable std::mutex mtx;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
  };


  int k;
  int sample_k;
  std::vector<LocalQueue> locals;
  mutable std::mt19937 rng;
};

#endif //DIJKSTRA_MULTITHREADED_SRC_DIJKSTRA_ALGORITHM_INCLUDE_DATA_STRUCTURES_CONCURRENT_KQUEUE_HPP_
