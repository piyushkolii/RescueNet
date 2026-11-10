#include "RescueNetSystem.h"

#include <iostream>

#include "Dijkstra.h"

namespace {
const char* statusName(RequestStatus s) {
    switch (s) {
        case RequestStatus::PENDING:   return "PENDING";
        case RequestStatus::ASSIGNED:  return "ASSIGNED";
        case RequestStatus::EVACUATED: return "EVACUATED";
        case RequestStatus::FAILED:    return "FAILED";
    }
    return "?";
}

const char* statusName(ReportStatus s) {
    switch (s) {
        case ReportStatus::PENDING:  return "PENDING";
        case ReportStatus::VERIFIED: return "VERIFIED";
        case ReportStatus::REJECTED: return "REJECTED";
    }
    return "?";
}
}

RescueNetSystem::RescueNetSystem()
    : nextRequestId(1), nextArrival(1), nextReportId(1) {}

bool RescueNetSystem::loadCity(const std::string& path) {
    if (!graph.loadFromFile(path)) return false;
    shelters.loadFromFile(path, graph);
    return true;
}

const Graph& RescueNetSystem::getGraph() const { return graph; }
const ShelterManager& RescueNetSystem::getShelters() const { return shelters; }
const std::vector<Assignment>& RescueNetSystem::getAssignments() const { return assignments; }

Route RescueNetSystem::findRoute(int fromId, int toId) const {
    return Dijkstra::shortestPath(graph, fromId, toId);
}

// ---------------- Evacuation requests ----------------

int RescueNetSystem::addRequest(int locationId, int groupSize, int dangerLevel, bool vulnerable) {
    if (!graph.getLocation(locationId) || groupSize <= 0) return -1;
    int id = nextRequestId++;
    requestQueue.push(EvacuationRequest(id, locationId, groupSize, dangerLevel,
                                        vulnerable, nextArrival++));
    return id;
}

int RescueNetSystem::pendingRequestCount() const { return requestQueue.size(); }

bool RescueNetSystem::processNextRequest() {
    if (requestQueue.empty()) return false;

    EvacuationRequest request = requestQueue.pop();
    Route route;
    int shelterId = shelters.findNearest(graph, request.getLocationId(),
                                         request.getGroupSize(), route);

    std::cout << "Request " << request.getId() << " (group of " << request.getGroupSize()
              << " at location " << request.getLocationId() << "): ";
    if (shelterId == -1) {
        request.markFailed();
        std::cout << "FAILED - no reachable shelter with enough space.\n";
    } else {
        shelters.getShelter(shelterId)->acceptEvacuees(request.getGroupSize());
        request.assignShelter(shelterId);
        std::cout << "assigned to shelter " << shelterId << ", route ";
        route.print();
    }
    assignments.push_back({request, route});
    return true;
}

int RescueNetSystem::processAllRequests() {
    int n = 0;
    while (processNextRequest()) ++n;
    return n;
}

bool RescueNetSystem::markEvacuated(int requestId) {
    for (Assignment& a : assignments) {
        if (a.request.getId() == requestId && a.request.getStatus() == RequestStatus::ASSIGNED) {
            a.request.markEvacuated();
            return true;
        }
    }
    return false;
}

// ---------------- Road reports ----------------

int RescueNetSystem::submitReport(int roadId, bool reportsBlocked,
                                  const std::string& reporter, const std::string& description) {
    if (!graph.getRoad(roadId)) return -1;
    int id = nextReportId++;
    reports.insert(RoadReport(id, roadId, reportsBlocked, reporter, description));
    reportQueue.enqueue(id);
    return id;
}

int RescueNetSystem::pendingReportCount() const { return reportQueue.size(); }

const RoadReport* RescueNetSystem::peekNextReport() const {
    if (reportQueue.empty()) return nullptr;
    return reports.find(reportQueue.front());
}

bool RescueNetSystem::decideNextReport(bool approve) {
    if (reportQueue.empty()) return false;
    RoadReport* report = reports.find(reportQueue.dequeue());
    if (!report) return false;

    if (!approve) {
        report->reject();
        std::cout << "Report " << report->getId() << " rejected. Map unchanged.\n";
        return true;
    }

    report->verify();
    Road* road = graph.getRoad(report->getRoadId());
    if (report->reportsBlocked()) {
        if (!road->isOpen()) {
            std::cout << "Road " << road->getId() << " was already blocked.\n";
            return true;
        }
        graph.blockRoad(road->getId());
        std::cout << "Road " << road->getId() << " marked BLOCKED.\n";
        rerouteAfterBlock(*road);
    } else {
        if (road->isOpen()) {
            std::cout << "Road " << road->getId() << " was already open.\n";
            return true;
        }
        graph.restoreRoad(road->getId());
        std::cout << "Road " << road->getId() << " restored to OPEN.\n";
        improveRoutesAfterRestore();
    }
    return true;
}

const RoadReport* RescueNetSystem::findReport(int reportId) const {
    return reports.find(reportId);
}

// ---------------- Rerouting ----------------

bool RescueNetSystem::routeUsesRoad(const Route& route, const Road& road) {
    const std::vector<int>& p = route.getPath();
    for (std::size_t i = 1; i < p.size(); ++i) {
        if ((p[i - 1] == road.getFrom() && p[i] == road.getTo()) ||
            (p[i - 1] == road.getTo() && p[i] == road.getFrom()))
            return true;
    }
    return false;
}

void RescueNetSystem::rerouteAfterBlock(const Road& road) {
    int affected = 0;
    for (Assignment& a : assignments) {
        EvacuationRequest& req = a.request;
        if (req.getStatus() != RequestStatus::ASSIGNED || !routeUsesRoad(a.route, road)) continue;
        ++affected;

        Shelter* current = shelters.getShelter(req.getAssignedShelterId());
        Route newRoute = findRoute(req.getLocationId(), current->getLocationId());
        std::cout << "  Rerouting request " << req.getId() << ": ";
        if (newRoute.isFound()) {
            a.route = newRoute;
            std::cout << "same shelter " << current->getId() << ", new route ";
            newRoute.print();
            continue;
        }

        // Assigned shelter is cut off: free the space and try another one.
        current->releaseEvacuees(req.getGroupSize());
        int otherId = shelters.findNearest(graph, req.getLocationId(), req.getGroupSize(),
                                           newRoute, current->getId());
        if (otherId == -1) {
            req.markFailed();
            a.route = Route();
            std::cout << "FAILED - no shelter reachable any more.\n";
        } else {
            shelters.getShelter(otherId)->acceptEvacuees(req.getGroupSize());
            req.assignShelter(otherId);
            a.route = newRoute;
            std::cout << "moved to shelter " << otherId << ", route ";
            newRoute.print();
        }
    }
    if (affected == 0) std::cout << "  No assigned routes used this road.\n";
}

void RescueNetSystem::improveRoutesAfterRestore() {
    for (Assignment& a : assignments) {
        if (a.request.getStatus() != RequestStatus::ASSIGNED) continue;
        const Shelter* s = shelters.getShelter(a.request.getAssignedShelterId());
        Route better = findRoute(a.request.getLocationId(), s->getLocationId());
        if (better.isFound() && better.getTotalCost() < a.route.getTotalCost()) {
            a.route = better;
            std::cout << "  Request " << a.request.getId() << " now has a shorter route ";
            better.print();
        }
    }
}

// ---------------- Display ----------------

void RescueNetSystem::printMap() const { graph.print(); }

void RescueNetSystem::printShelters() const { shelters.print(); }

void RescueNetSystem::printPendingRequests() const { requestQueue.print(); }

void RescueNetSystem::printAssignments() const {
    if (assignments.empty()) {
        std::cout << "No requests processed yet.\n";
        return;
    }
    for (const Assignment& a : assignments) {
        const EvacuationRequest& r = a.request;
        std::cout << "  Request " << r.getId() << " | location " << r.getLocationId()
                  << " | group " << r.getGroupSize() << " | " << statusName(r.getStatus());
        if (r.getStatus() == RequestStatus::FAILED) {
            std::cout << "\n";
        } else {
            std::cout << " | shelter " << r.getAssignedShelterId() << " | route ";
            a.route.print();
        }
    }
}

void RescueNetSystem::printReports() const {
    std::vector<const RoadReport*> list = reports.all();
    if (list.empty()) {
        std::cout << "No road reports yet.\n";
        return;
    }
    for (const RoadReport* r : list) {
        std::cout << "  Report " << r->getId() << " | road " << r->getRoadId()
                  << " | " << (r->reportsBlocked() ? "BLOCKED" : "CLEARED")
                  << " | by " << r->getReporter()
                  << " | " << statusName(r->getStatus())
                  << " | " << r->getDescription() << "\n";
    }
}
