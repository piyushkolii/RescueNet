#pragma once

#include <vector>

#include "Graph.h"
#include "Route.h"

class Dijkstra {
public:

    static bool run(const Graph& graph, int sourceId,
                    std::vector<double>& dist, std::vector<int>& prev);

    // Builds the Route to targetId from a finished run(). Empty if unreachable.
    static Route buildRoute(const Graph& graph, int targetId,
                            const std::vector<double>& dist, const std::vector<int>& prev);

    // run() + buildRoute() for a single source/destination pair.
    static Route shortestPath(const Graph& graph, int fromId, int toId);
};
