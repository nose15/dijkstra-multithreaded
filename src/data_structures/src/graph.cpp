//
// Created by lukasz on 25.11.25.
//

#include <data_structures/graph.hpp>

Graph::Graph(int n) : m_adj(n) {}


int Graph::size() const { return (int)m_adj.size(); }


void Graph::add_edge(int u, int v, double w) {
  m_adj[u].push_back({v, w});
}


const std::vector<std::vector<Edge>>& Graph::adj() const { return m_adj; }