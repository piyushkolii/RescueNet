#include "Route.h"

#include <iostream>

Route::Route() : path(), totalCost(0.0) {}

Route::Route(const std::vector<int>& path_, double totalCost_)
    : path(path_), totalCost(totalCost_) {}

const std::vector<int>& Route::getPath() const { return path; }
double Route::getTotalCost() const { return totalCost; }
bool Route::isFound() const { return !path.empty(); }

void Route::print() const {
    if (!isFound()) {
        std::cout << "No route available\n";
        return;
    }
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i > 0) std::cout << " -> ";
        std::cout << path[i];
    }
    std::cout << " (cost " << totalCost << ")\n";
}
