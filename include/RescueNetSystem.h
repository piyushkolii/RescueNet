#pragma once

#include <string>
#include <vector>

#include "EvacuationQueue.h"
#include "EvacuationRequest.h"
#include "Graph.h"
#include "ReportHashTable.h"
#include "ReportQueue.h"
#include "Route.h"
#include "ShelterManager.h"

// A processed request together with the route it was given.
struct Assignment {
    EvacuationRequest request;
    Route route;
};

// Ties every module together: the road graph, routing, the evacuation
// priority queue, shelter allocation and the road-report review workflow.
class RescueNetSystem {
public:
    RescueNetSystem();

    // Loads LOCATION, ROAD and SHELTER lines from one map file.
    bool loadCity(const std::string& path);

    const Graph& getGraph() const;
    const ShelterManager& getShelters() const;
    const std::vector<Assignment>& getAssignments() const;

    Route findRoute(int fromId, int toId) const;

    // --- Evacuation requests ---
    // Returns the new request ID, or -1 if the input is invalid.
    int addRequest(int locationId, int groupSize, int dangerLevel, bool vulnerable);
    int pendingRequestCount() const;
    // Serves the most urgent request; false if the queue is empty.
    bool processNextRequest();
    int processAllRequests();
    bool markEvacuated(int requestId);

    // --- Road reports ---
    // Returns the new report ID, or -1 if the road does not exist.
    int submitReport(int roadId, bool reportsBlocked,
                     const std::string& reporter, const std::string& description);
    int pendingReportCount() const;
    const RoadReport* peekNextReport() const;
    // Verifies (approve = true) or rejects the oldest pending report.
    // A verified report updates the graph and reroutes affected groups.
    bool decideNextReport(bool approve);
    const RoadReport* findReport(int reportId) const;

    // --- Display ---
    void printMap() const;
    void printShelters() const;
    void printPendingRequests() const;
    void printAssignments() const;
    void printReports() const;

private:
    Graph graph;
    ShelterManager shelters;
    EvacuationQueue requestQueue;
    std::vector<Assignment> assignments;
    ReportQueue reportQueue;
    ReportHashTable reports;
    int nextRequestId;
    int nextArrival;
    int nextReportId;

    void rerouteAfterBlock(const Road& road);
    void improveRoutesAfterRestore();
    static bool routeUsesRoad(const Route& route, const Road& road);
};
