// Simple self-checking tests for every module. Run from the project root so
// data/city.txt can be found.
#include <iostream>
#include <string>
#include <vector>

#include "Dijkstra.h"
#include "EvacuationRequest.h"
#include "Graph.h"
#include "MinHeap.h"
#include "Shelter.h"
#include "ShelterManager.h"

namespace {
int passed = 0;
int failed = 0;

void check(bool ok, const std::string& name) {
    if (ok) {
        ++passed;
    } else {
        ++failed;
        std::cout << "FAIL: " << name << "\n";
    }
}

const std::string MAP = "data/city.txt";
}  // namespace

void testGraph() {
    Graph g;
    check(g.loadFromFile(MAP), "graph loads");
    check(g.locationCount() == 12, "12 locations");
    check(g.roadCount() == 17, "17 roads");
    check(g.getLocation(10) && g.getLocation(10)->getName() == "City_Stadium", "location lookup");
    check(!g.addRoad(Road(99, 1, 999, 1.0)), "road to unknown location rejected");
    check(!g.addRoad(Road(1, 1, 2, 1.0)), "duplicate road id rejected");

    int degree = 0;
    for (const Graph::EdgeNode* e = g.neighbours(g.indexOf(1)); e; e = e->next) ++degree;
    check(degree == 3, "Central_Market has 3 roads");
}

void testMinHeap() {
    MinHeap h;
    double values[] = {5, 1, 9, 3, 7, 2};
    for (int i = 0; i < 6; ++i) h.push(values[i], i);
    std::vector<double> out;
    while (!h.empty()) out.push_back(h.pop().distance);
    check(out == std::vector<double>({1, 2, 3, 5, 7, 9}), "min-heap pops in order");
}

void testDijkstra() {
    Graph g;
    g.loadFromFile(MAP);

    Route r = Dijkstra::shortestPath(g, 1, 10);
    check(r.isFound() && r.getTotalCost() == 11, "1 -> 10 costs 11");
    check(r.getPath() == std::vector<int>({1, 2, 10}), "1 -> 10 goes via 2");

    g.blockRoad(5);   // 2 <-> 10
    r = Dijkstra::shortestPath(g, 1, 10);
    check(r.getTotalCost() == 14, "detour cost 14 after blocking road 5");
    check(r.getPath() == std::vector<int>({1, 8, 7, 10}), "detour goes via 8 and 7");

    g.restoreRoad(5);
    check(Dijkstra::shortestPath(g, 1, 10).getTotalCost() == 11, "restored road used again");

    Route self = Dijkstra::shortestPath(g, 3, 3);
    check(self.isFound() && self.getTotalCost() == 0, "route to self is zero");

    g.blockRoad(11);  // 6 <-> 12
    g.blockRoad(10);  // 6 <-> 8, Industrial_Area is now cut off
    check(!Dijkstra::shortestPath(g, 6, 1).isFound(), "no route from isolated location");
    check(!Dijkstra::shortestPath(g, 1, 999).isFound(), "unknown destination gives no route");
}

void testEvacuationRequest() {
    EvacuationRequest r(1, 1, 10, 3, true, 1);
    check(r.getPriority() == 3510, "priority = danger*1000 + vulnerable 500 + group");
    check(EvacuationRequest(2, 1, 10, 9, false, 2).getDangerLevel() == 5, "danger level capped at 5");
    check(r.getStatus() == RequestStatus::PENDING && r.getAssignedShelterId() == -1,
          "new request is pending");

    EvacuationRequest early(3, 1, 10, 3, false, 3);
    EvacuationRequest late(4, 1, 10, 3, false, 4);
    check(early.hasHigherPriorityThan(late) && !late.hasHigherPriorityThan(early),
          "equal priority: earlier arrival first");

    r.assignShelter(2);
    check(r.getStatus() == RequestStatus::ASSIGNED && r.getAssignedShelterId() == 2,
          "assignShelter sets shelter and status");
}

void testShelter() {
    Shelter s(1, "Test_Shelter", 10, 100);
    check(s.acceptEvacuees(60) && s.getOccupancy() == 60, "group of 60 accepted");
    check(!s.acceptEvacuees(50) && s.getOccupancy() == 60, "group too big refused, nothing changes");
    s.releaseEvacuees(20);
    check(s.getOccupancy() == 40 && s.getRemainingCapacity() == 60, "release gives space back");
    s.releaseEvacuees(100);
    check(s.getOccupancy() == 0, "release never goes below zero");
}

void testShelterAllocation() {
    Graph g;
    g.loadFromFile(MAP);
    ShelterManager sm;
    check(sm.loadFromFile(MAP, g) && sm.count() == 3, "3 shelters load");

    Route r;
    int id = sm.findNearest(g, 5, 100, r);
    check(id == 2 && r.getTotalCost() == 4, "Hill_View goes to Hall_Shelter");
    sm.getShelter(id)->acceptEvacuees(100);

    id = sm.findNearest(g, 5, 100, r);
    check(id == 3, "full shelter skipped, next nearest chosen");

    check(sm.findNearest(g, 5, 10000, r) == -1, "no shelter big enough");
}

int main() {
    testGraph();
    testMinHeap();
    testDijkstra();
    testEvacuationRequest();
    testShelter();
    testShelterAllocation();

    std::cout << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
