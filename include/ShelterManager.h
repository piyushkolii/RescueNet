#pragma once

#include <string>
#include <vector>

#include "Graph.h"
#include "Route.h"
#include "Shelter.h"

// Owns the shelters and picks the nearest one that can take a whole group.
class ShelterManager {
public:
    ShelterManager();

    // Reads SHELTER lines from the map file; other lines are skipped.
    // Format:  SHELTER <id> <name> <locationId> <capacity>
    bool loadFromFile(const std::string& path, const Graph& graph);

    bool addShelter(const Shelter& shelter, const Graph& graph);
    void clear();

    Shelter* getShelter(int shelterId);
    const Shelter* getShelter(int shelterId) const;
    const std::vector<Shelter>& getShelters() const;
    int count() const;

    // Nearest reachable shelter (over OPEN roads) with room for groupSize.
    // Returns its id and fills outRoute, or returns -1 if none fits.
    // excludeShelterId lets rerouting skip the shelter that became unreachable.
    int findNearest(const Graph& graph, int fromLocationId, int groupSize,
                    Route& outRoute, int excludeShelterId = -1) const;

    void print() const;

private:
    std::vector<Shelter> shelters;
};
