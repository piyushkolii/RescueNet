#include "Graph.h"

#include <fstream>
#include <iostream>
#include <sstream>

Graph::Graph() {}

Graph::~Graph() { clear(); }

void Graph::clear() {
    for (EdgeNode* head : adjacency) {
        while (head) {
            EdgeNode* next = head->next;
            delete head;
            head = next;
        }
    }
    adjacency.clear();
    locations.clear();
    roads.clear();
}

bool Graph::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Could not open map file: " << path << "\n";
        return false;
    }

    clear();
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        std::istringstream ss(line);
        std::string keyword;
        if (!(ss >> keyword) || keyword[0] == '#') continue;

        if (keyword == "LOCATION") {
            int id;
            std::string name, type;
            if (!(ss >> id >> name >> type) || !addLocation(Location(id, name, type)))
                std::cout << "Skipping bad LOCATION on line " << lineNo << "\n";
        } else if (keyword == "ROAD") {
            int id, from, to;
            double weight;
            if (!(ss >> id >> from >> to >> weight) || weight < 0 ||
                !addRoad(Road(id, from, to, weight)))
                std::cout << "Skipping bad ROAD on line " << lineNo << "\n";
        }
    }
    return !locations.empty();
}

bool Graph::addLocation(const Location& location) {
    if (indexOf(location.getId()) != -1) return false;
    locations.push_back(location);
    adjacency.push_back(nullptr);
    return true;
}

bool Graph::addRoad(const Road& road) {
    int a = indexOf(road.getFrom());
    int b = indexOf(road.getTo());
    if (a == -1 || b == -1 || a == b || roadIndexOf(road.getId()) != -1) return false;

    roads.push_back(road);
    int r = static_cast<int>(roads.size()) - 1;
    pushEdge(a, r);
    pushEdge(b, r);
    return true;
}

void Graph::pushEdge(int locationIndex, int roadIndex) {
    adjacency[locationIndex] = new EdgeNode{roadIndex, adjacency[locationIndex]};
}

int Graph::locationCount() const { return static_cast<int>(locations.size()); }
int Graph::roadCount() const { return static_cast<int>(roads.size()); }

int Graph::indexOf(int locationId) const {
    for (std::size_t i = 0; i < locations.size(); ++i)
        if (locations[i].getId() == locationId) return static_cast<int>(i);
    return -1;
}

int Graph::roadIndexOf(int roadId) const {
    for (std::size_t i = 0; i < roads.size(); ++i)
        if (roads[i].getId() == roadId) return static_cast<int>(i);
    return -1;
}

const Location* Graph::getLocation(int locationId) const {
    int i = indexOf(locationId);
    return i == -1 ? nullptr : &locations[i];
}

const Location& Graph::locationAt(int index) const { return locations[index]; }

Road* Graph::getRoad(int roadId) {
    int i = roadIndexOf(roadId);
    return i == -1 ? nullptr : &roads[i];
}

const Road* Graph::getRoad(int roadId) const {
    int i = roadIndexOf(roadId);
    return i == -1 ? nullptr : &roads[i];
}

const Road& Graph::roadAt(int index) const { return roads[index]; }
const std::vector<Location>& Graph::getLocations() const { return locations; }
const std::vector<Road>& Graph::getRoads() const { return roads; }

const Graph::EdgeNode* Graph::neighbours(int index) const { return adjacency[index]; }

bool Graph::blockRoad(int roadId) {
    Road* road = getRoad(roadId);
    if (!road) return false;
    road->block();
    return true;
}

bool Graph::restoreRoad(int roadId) {
    Road* road = getRoad(roadId);
    if (!road) return false;
    road->restore();
    return true;
}

void Graph::print() const {
    std::cout << "\nLocations (" << locations.size() << "):\n";
    for (std::size_t i = 0; i < locations.size(); ++i) {
        const Location& loc = locations[i];
        std::cout << "  [" << loc.getId() << "] " << loc.getName() << " (" << loc.getType() << ") ->";
        for (const EdgeNode* e = adjacency[i]; e; e = e->next) {
            const Road& road = roads[e->roadIndex];
            int other = road.getFrom() == loc.getId() ? road.getTo() : road.getFrom();
            std::cout << " " << other << "(" << road.getWeight()
                      << (road.isOpen() ? "" : ", BLOCKED") << ")";
        }
        std::cout << "\n";
    }

    std::cout << "Roads (" << roads.size() << "):\n";
    for (const Road& road : roads) {
        std::cout << "  Road " << road.getId() << ": " << road.getFrom() << " <-> " << road.getTo()
                  << "  weight " << road.getWeight()
                  << "  " << (road.isOpen() ? "OPEN" : "BLOCKED") << "\n";
    }
}