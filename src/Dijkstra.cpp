#include "Dijkstra.h"

#include <algorithm>
#include <limits>

#include "MinHeap.h"

namespace {
const double INF = std::numeric_limits<double>::infinity();
}

bool Dijkstra::run(const Graph& graph, int sourceId,
                   std::vector<double>& dist, std::vector<int>& prev) {
    int n = graph.locationCount();
    dist.assign(n, INF);
    prev.assign(n, -1);

    int source = graph.indexOf(sourceId);
    if (source == -1) return false;

    std::vector<bool> done(n, false);
    MinHeap heap;
    dist[source] = 0.0;
    heap.push(0.0, source);

    while (!heap.empty()) {
        MinHeap::Entry current = heap.pop();
        int u = current.node;
        if (done[u]) continue;   // stale entry left over from an earlier relax
        done[u] = true;

        int uId = graph.locationAt(u).getId();
        for (const Graph::EdgeNode* e = graph.neighbours(u); e; e = e->next) {
            const Road& road = graph.roadAt(e->roadIndex);
            if (!road.isOpen()) continue;

            int vId = road.getFrom() == uId ? road.getTo() : road.getFrom();
            int v = graph.indexOf(vId);
            double candidate = dist[u] + road.getWeight();
            if (!done[v] && candidate < dist[v]) {
                dist[v] = candidate;
                prev[v] = u;
                heap.push(candidate, v);
            }
        }
    }
    return true;
}

Route Dijkstra::buildRoute(const Graph& graph, int targetId,
                           const std::vector<double>& dist, const std::vector<int>& prev) {
    int target = graph.indexOf(targetId);
    if (target == -1 || target >= static_cast<int>(dist.size()) || dist[target] == INF)
        return Route();

    std::vector<int> path;
    for (int at = target; at != -1; at = prev[at])
        path.push_back(graph.locationAt(at).getId());
    std::reverse(path.begin(), path.end());
    return Route(path, dist[target]);
}

Route Dijkstra::shortestPath(const Graph& graph, int fromId, int toId) {
    std::vector<double> dist;
    std::vector<int> prev;
    if (!run(graph, fromId, dist, prev)) return Route();
    return buildRoute(graph, toId, dist, prev);
}
