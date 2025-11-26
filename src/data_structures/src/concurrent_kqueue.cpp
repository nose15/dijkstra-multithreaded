//
// Created by lukasz on 25.11.25.
//

#include <algorithm>
#include <thread>
#include <data_structures/concurrent_kqueue.hpp>

ConcurrentKQueue::ConcurrentKQueue(int partitions, int sample_k_)
    : k(std::max(1, partitions)),
      sample_k(std::max(1, std::min(sample_k_, partitions))),
      locals(k)
{}

void ConcurrentKQueue::push(PQKey key, PQVal val) {
  // Hash-based partitioning
  int idx = (std::hash<PQVal>{}(val) & 0x7fffffff) % k;
  auto &L = locals[idx];
  std::lock_guard<std::mutex> lk(L.mtx);
  L.pq.emplace(key, val);
}

std::optional<ConcurrentKQueue::PQItem> ConcurrentKQueue::pop() {
  // SAFETY: thread-local RNG to avoid data races
  static thread_local std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> dist(0, k - 1);

  // --- Sample distinct indices ---
  std::vector<int> sampled;
  sampled.reserve(sample_k);
  while ((int)sampled.size() < sample_k) {
    int r = dist(rng);
    if (std::find(sampled.begin(), sampled.end(), r) == sampled.end())
      sampled.push_back(r);
  }

  PQItem best{std::numeric_limits<PQKey>::infinity(), -1};
  int best_idx = -1;

  // Find best among sampled
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

  // If found nothing, try all queues
  if (best_idx == -1) {
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

  if (best_idx == -1)
    return std::nullopt;

  // Now pop the chosen one
  auto &Chosen = locals[best_idx];
  std::lock_guard<std::mutex> lk(Chosen.mtx);
  if (Chosen.pq.empty())
    return std::nullopt;

  auto out = Chosen.pq.top();
  Chosen.pq.pop();
  return out;
}

bool ConcurrentKQueue::empty() const {
  for (int i = 0; i < k; ++i) {
    auto &L = locals[i];
    std::lock_guard<std::mutex> lk(L.mtx);
    if (!L.pq.empty())
      return false;
  }
  return true;
}

