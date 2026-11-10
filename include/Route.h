#pragma once

#include <vector>

class Route {
public:
    Route();
    Route(const std::vector<int>& path, double totalCost);

    const std::vector<int>& getPath() const;
    double getTotalCost() const;
    bool isFound() const;   
    void print() const;       

private:
    std::vector<int> path;
    double totalCost;
};
