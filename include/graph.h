#pragma once

#include <string>
#include <vector>
#include "Location.h"
#include "Road.h"
class Graph {
public:
    struct EdgeNode {
        int roadIndex; 
        EdgeNode* next;
    };

    Graph();
    ~Graph();
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    bool loadFromFile(const std::string& path);

    bool addLocation(const Location& location);
    bool addRoad(const Road& road);             
    void clear();

    int locationCount() const;
    int roadCount() const;
    int indexOf(int locationId) const;  

    const Location* getLocation(int locationId) const;
    const Location& locationAt(int index) const;
    Road* getRoad(int roadId);
    const Road* getRoad(int roadId) const;
    const Road& roadAt(int index) const;
    const std::vector<Location>& getLocations() const;
    const std::vector<Road>& getRoads() const;

    const EdgeNode* neighbours(int index) const;

    bool blockRoad(int roadId);
    bool restoreRoad(int roadId);

    void print() const;

private:
    std::vector<Location> locations;
    std::vector<Road> roads;
    std::vector<EdgeNode*> adjacency; 

    void pushEdge(int locationIndex, int roadIndex);
    int roadIndexOf(int roadId) const;
};