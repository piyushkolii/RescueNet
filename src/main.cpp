#include <iostream>
#include <string>
#include <vector>

#include "Dijkstra.h"
#include "EvacuationQueue.h"
#include "EvacuationRequest.h"
#include "Graph.h"
#include "Location.h"
#include "Road.h"
#include "RoadReport.h"
#include "Route.h"
#include "ShelterManager.h"

// Demo menu. The city map and the shelters come from data/city.txt. Evacuation
// requests wait in a priority queue and are sent to the nearest shelter with
// room; road reports are still a plain vector until their own milestone.
const std::string MAP_FILE = "data/city.txt";

Graph city;
ShelterManager shelters;
EvacuationQueue pending;
std::vector<EvacuationRequest> processed;
std::vector<RoadReport> reports;
int nextRequestId = 1;

int readInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Enter a number: ";
    }
    return value;
}

void loadCity() {
    if (city.loadFromFile(MAP_FILE) && shelters.loadFromFile(MAP_FILE, city))
        std::cout << "Loaded " << city.locationCount() << " locations, " << city.roadCount()
                  << " roads and " << shelters.count() << " shelters from " << MAP_FILE << ".\n";
}

const char* statusName(RequestStatus status) {
    switch (status) {
        case RequestStatus::PENDING: return "PENDING";
        case RequestStatus::ASSIGNED: return "ASSIGNED";
        case RequestStatus::EVACUATED: return "EVACUATED";
        case RequestStatus::FAILED: return "FAILED";
    }
    return "";
}

// Takes the most urgent request and sends it to the nearest shelter that can
// take the whole group.
void serveNextRequest() {
    if (pending.empty()) {
        std::cout << "No pending requests.\n";
        return;
    }
    EvacuationRequest r = pending.pop();
    Route route;
    int shelterId = shelters.findNearest(city, r.getLocationId(), r.getGroupSize(), route);
    if (shelterId == -1) {
        r.markFailed();
        std::cout << "Request " << r.getId() << ": no reachable shelter has room for "
                  << r.getGroupSize() << " people.\n";
    } else {
        Shelter* s = shelters.getShelter(shelterId);
        s->acceptEvacuees(r.getGroupSize());
        r.assignShelter(shelterId);
        std::cout << "Request " << r.getId() << " -> " << s->getName() << ": ";
        route.print();
    }
    processed.push_back(r);
}

int main() {
    loadCity();

    while (true) {
        std::cout << "\n===== RescueNet =====\n"
                  << "1. Show locations\n2. Show roads\n3. Show shelters\n"
                  << "4. Add evacuation request\n5. Show pending requests (priority order)\n"
                  << "6. Add road report\n7. Show road reports\n"
                  << "8. Reload city map (resets shelters)\n9. Show map (adjacency list)\n"
                  << "10. Find shortest route\n11. Block road\n12. Restore road\n"
                  << "13. Serve next request\n14. Show processed requests\n0. Exit\n";
        int choice = readInt("Choose: ");

        if (choice == 0) break;
        if (choice == 1) {
            for (const Location& l : city.getLocations())
                std::cout << "  [" << l.getId() << "] " << l.getName() << " (" << l.getType() << ")\n";
        } else if (choice == 2) {
            for (const Road& r : city.getRoads())
                std::cout << "  Road " << r.getId() << ": " << r.getFrom() << " <-> " << r.getTo()
                          << ", weight " << r.getWeight() << (r.isOpen() ? ", OPEN\n" : ", BLOCKED\n");
        } else if (choice == 3) {
            shelters.print();
        } else if (choice == 4) {
            int location = readInt("Location ID: ");
            if (!city.getLocation(location)) {
                std::cout << "Unknown location.\n";
                continue;
            }
            int people = readInt("Number of people: ");
            int danger = readInt("Danger level (1-5): ");
            bool vulnerable = readInt("Vulnerable people? (1 = yes, 0 = no): ") == 1;
            pending.push(EvacuationRequest(nextRequestId, location, people, danger, vulnerable,
                                           nextRequestId));
            std::cout << "Request " << nextRequestId << " added to the priority queue.\n";
            ++nextRequestId;
        } else if (choice == 5) {
            pending.print();
        } else if (choice == 6) {
            int id = static_cast<int>(reports.size()) + 1;
            int road = readInt("Road ID: ");
            bool blocked = readInt("1 = blocked, 2 = cleared: ") == 1;
            std::string reporter;
            std::cout << "Reporter name: ";
            std::cin >> reporter;
            reports.push_back(RoadReport(id, road, blocked, reporter, ""));
            std::cout << "Report " << id << " added (pending).\n";
        } else if (choice == 7) {
            for (const RoadReport& r : reports)
                std::cout << "  Report " << r.getId() << ": road " << r.getRoadId()
                          << (r.reportsBlocked() ? " blocked" : " cleared") << ", by " << r.getReporter()
                          << (r.isPending() ? " [PENDING]\n" : " [DECIDED]\n");
        } else if (choice == 8) {
            loadCity();
        } else if (choice == 9) {
            city.print();
        } else if (choice == 10) {
            int from = readInt("From location ID: ");
            int to = readInt("To location ID: ");
            Dijkstra::shortestPath(city, from, to).print();
        } else if (choice == 11) {
            int road = readInt("Road ID to block: ");
            std::cout << (city.blockRoad(road) ? "Road blocked.\n" : "No such road.\n");
        } else if (choice == 12) {
            int road = readInt("Road ID to restore: ");
            std::cout << (city.restoreRoad(road) ? "Road restored.\n" : "No such road.\n");
        } else if (choice == 13) {
            serveNextRequest();
        } else if (choice == 14) {
            if (processed.empty()) std::cout << "No processed requests.\n";
            for (const EvacuationRequest& r : processed) {
                std::cout << "  Request " << r.getId() << ": location " << r.getLocationId() << ", "
                          << r.getGroupSize() << " people, " << statusName(r.getStatus());
                if (r.getStatus() == RequestStatus::ASSIGNED)
                    std::cout << " to shelter " << r.getAssignedShelterId();
                std::cout << "\n";
            }
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
    std::cout << "Goodbye.\n";
}
