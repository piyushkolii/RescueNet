#pragma once

#include <vector>

// Result of a routing query: the ordered Location IDs from source to
// destination and the total cost. An empty route means no path was found.
class Route {
public:
    Route();
    Route(const std::vector<int>& path, double totalCost);

    const std::vector<int>& getPath() const;
    double getTotalCost() const;
    bool isFound() const;     // false when no route is available
    void print() const;       // e.g. "1 -> 4 -> 7 (cost 12.5)" or "No route available"

private:
    std::vector<int> path;
    double totalCost;
};
