import heapq
import random
import time
from multiprocessing import Pool, cpu_count


# --- Generowanie losowego grafu ---
def generate_graph(num_nodes: int, density: float):
    """
    density: 0.0 - 1.0 (0% - 100%)
    """
    graph = {i: {} for i in range(num_nodes)}
    max_edges = num_nodes * (num_nodes - 1) // 2
    num_edges = int(max_edges * density)

    edges = set()
    while len(edges) < num_edges:
        u = random.randint(0, num_nodes - 1)
        v = random.randint(0, num_nodes - 1)
        if u != v and (u, v) not in edges and (v, u) not in edges:
            weight = random.randint(1, 20)
            graph[u][v] = weight
            graph[v][u] = weight
            edges.add((u, v))
    return graph


# --- Algorytm Dijkstry ---
def dijkstra(args):
    graph, start = args
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    pq = [(0, start)]

    while pq:
        current_distance, current_node = heapq.heappop(pq)
        if current_distance > distances[current_node]:
            continue
        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                heapq.heappush(pq, (distance, neighbor))
    return start, distances


# --- Wersja wieloprocesowa ---
def multi_process(graph, sources, num_processes):
    with Pool(processes=num_processes) as pool:
        results = dict(pool.map(dijkstra, [(graph, s) for s in sources]))
    return results


# --- Benchmark ---
def benchmark():
    node_counts = [100, 500, 1000]      # liczba wierzchołków
    densities = [0.1, 0.3, 0.6]         # gęstość grafu
    process_counts = [1, 2, 4, 8]       # liczba procesów
    sources = list(range(10))           # źródła

    print(f"{'Wierzch.':>10} | {'Gęstość':>8} | {'Procesy':>8} | {'Czas [s]':>10}")
    print("-" * 50)

    for n in node_counts:
        for d in densities:
            graph = generate_graph(n, d)
            for p in process_counts:
                start = time.time()
                multi_process(graph, sources, p)
                elapsed = time.time() - start
                print(f"{n:>10} | {d:>8.2f} | {p:>8} | {elapsed:>10.4f}")


if __name__ == "__main__":
    random.seed(42)
    print(f"CPU dostępne: {cpu_count()}\n")
    benchmark()
