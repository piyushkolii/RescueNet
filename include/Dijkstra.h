#pragma once

#include <vector>

#include "Graph.h"
#include "Route.h"

// Shortest paths over OPEN roads only; BLOCKED roads are skipped entirely.
class Dijkstra {
public:
    // Fills dist/prev (indexed by location index) for every location reachable
    // from sourceId. Unreachable locations keep dist = infinity, prev = -1.
    // Returns false if sourceId is not in the graph.
    static bool run(const Graph& graph, int sourceId,
                    std::vector<double>& dist, std::vector<int>& prev);

    // Builds the Route to targetId from a finished run(). Empty if unreachable.
    static Route buildRoute(const Graph& graph, int targetId,
                            const std::vector<double>& dist, const std::vector<int>& prev);

    // run() + buildRoute() for a single source/destination pair.
    static Route shortestPath(const Graph& graph, int fromId, int toId);
};
