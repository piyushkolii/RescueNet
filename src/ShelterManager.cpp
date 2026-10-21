#include "ShelterManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Dijkstra.h"

ShelterManager::ShelterManager() : shelters() {}

bool ShelterManager::loadFromFile(const std::string& path, const Graph& graph) {
    std::ifstream in(path);
    if (!in) return false;

    clear();
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        std::istringstream ss(line);
        std::string keyword;
        if (!(ss >> keyword) || keyword != "SHELTER") continue;

        int id, locationId, capacity;
        std::string name;
        if (!(ss >> id >> name >> locationId >> capacity) ||
            !addShelter(Shelter(id, name, locationId, capacity), graph))
            std::cout << "Skipping bad SHELTER on line " << lineNo << "\n";
    }
    return !shelters.empty();
}

bool ShelterManager::addShelter(const Shelter& shelter, const Graph& graph) {
    if (getShelter(shelter.getId()) || !graph.getLocation(shelter.getLocationId())) return false;
    shelters.push_back(shelter);
    return true;
}

void ShelterManager::clear() { shelters.clear(); }

Shelter* ShelterManager::getShelter(int shelterId) {
    for (Shelter& s : shelters)
        if (s.getId() == shelterId) return &s;
    return nullptr;
}

const Shelter* ShelterManager::getShelter(int shelterId) const {
    for (const Shelter& s : shelters)
        if (s.getId() == shelterId) return &s;
    return nullptr;
}

const std::vector<Shelter>& ShelterManager::getShelters() const { return shelters; }
int ShelterManager::count() const { return static_cast<int>(shelters.size()); }

int ShelterManager::findNearest(const Graph& graph, int fromLocationId, int groupSize,
                                Route& outRoute, int excludeShelterId) const {
    std::vector<double> dist;
    std::vector<int> prev;
    if (!Dijkstra::run(graph, fromLocationId, dist, prev)) return -1;

    int bestId = -1;
    Route bestRoute;
    for (const Shelter& s : shelters) {
        if (s.getId() == excludeShelterId || !s.canAccommodate(groupSize)) continue;
        Route route = Dijkstra::buildRoute(graph, s.getLocationId(), dist, prev);
        if (!route.isFound()) continue;
        if (bestId == -1 || route.getTotalCost() < bestRoute.getTotalCost()) {
            bestId = s.getId();
            bestRoute = route;
        }
    }
    outRoute = bestRoute;
    return bestId;
}

void ShelterManager::print() const {
    if (shelters.empty()) {
        std::cout << "No shelters loaded.\n";
        return;
    }
    for (const Shelter& s : shelters) {
        std::cout << "  Shelter " << s.getId() << ": " << s.getName()
                  << " at location " << s.getLocationId()
                  << " | " << s.getOccupancy() << "/" << s.getCapacity()
                  << (s.isFull() ? " FULL" : "") << "\n";
    }
}
