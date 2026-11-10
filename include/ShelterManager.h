#pragma once

#include <string>
#include <vector>

#include "Graph.h"
#include "Route.h"
#include "Shelter.h"
class ShelterManager {
public:
    ShelterManager();


    bool loadFromFile(const std::string& path, const Graph& graph);

    bool addShelter(const Shelter& shelter, const Graph& graph);
    void clear();

    Shelter* getShelter(int shelterId);
    const Shelter* getShelter(int shelterId) const;
    const std::vector<Shelter>& getShelters() const;
    int count() const;

    int findNearest(const Graph& graph, int fromLocationId, int groupSize,
                    Route& outRoute, int excludeShelterId = -1) const;

    void print() const;

private:
    std::vector<Shelter> shelters;
};
