//
// Created by lukasz on 25.11.25.
//

#include <data_structures/concurrent_kqueue.hpp>

#include <algorithm>

ConcurrentKQueue::ConcurrentKQueue(int partitions, int sample_k_)
    : k(std::max(1, partitions)), sample_k(std::max(1, std::min(sample_k_, partitions))),
      locals(k), rng(std::random_device{}()) {}

void ConcurrentKQueue::push(PQKey key, PQVal val) {
// simple hash by val to choose a local queue to reduce contention
  int idx = (std::hash<PQVal>{}(val) & 0x7fffffff) % k;
  auto &L = locals[idx];
  std::lock_guard<std::mutex> lk(L.mtx);
  L.pq.emplace(key, val);
}

std::optional<PQItem> ConcurrentKQueue::pop() {
// sample sample_k distinct indices
  std::uniform_int_distribution<int> dist(0, k - 1);
  std::vector<int> sampled;
  sampled.reserve(sample_k);
  while ((int)sampled.size() < sample_k) {
    int r = dist(rng);
    if (std::find(sampled.begin(), sampled.end(), r) == sampled.end()) sampled.push_back(r);
  }


// gather tops while holding locks briefly
  PQItem best{std::numeric_limits<PQKey>::infinity(), -1};
  int best_idx = -1;


// We'll lock each sampled queue, check its top, and remember the best
  for (int idx : sampled) {
    auto &L = locals[idx];
    std::lock_guard<std::mutex> lk(L.mtx);
    if (!L.pq.empty()) {
      const PQItem &t = L.pq.top();
      if (t.first < best.first) {
        best = t;
        best_idx = idx;
      }
    }
  }


  if (best_idx == -1) {
// none of sampled queues had elements; try to find any non-empty quickly
    for (int i = 0; i < k; ++i) {
      auto &L = locals[i];
      std::lock_guard<std::mutex> lk(L.mtx);
      if (!L.pq.empty()) {
        best = L.pq.top();
        best_idx = i;
        break;
      }
    }
  }


  if (best_idx == -1) return std::nullopt; // empty


// Now pop from the chosen queue under lock
  auto &Chosen = locals[best_idx];
  std::lock_guard<std::mutex> lk(Chosen.mtx);
  if (Chosen.pq.empty()) return std::nullopt; // race
  auto out = Chosen.pq.top();
  Chosen.pq.pop();
  return out;
}


bool ConcurrentKQueue::empty() const {
  for (int i = 0; i < k; ++i) {
    auto &L = locals[i];
    std::lock_guard<std::mutex> lk(L.mtx);
    if (!L.pq.empty()) return false;
  }
  return true;
}

