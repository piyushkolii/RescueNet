#include <iostream>
#include <string>
#include <vector>

#include "Dijkstra.h"
#include "EvacuationRequest.h"
#include "Graph.h"
#include "Location.h"
#include "Road.h"
#include "RoadReport.h"
#include "Route.h"
#include "Shelter.h"

// Demo menu. The city map (locations + roads) now comes from data/city.txt
// through the Graph; shelters, requests and reports are still plain vectors
// until their own milestones replace them.
const std::string MAP_FILE = "data/city.txt";

Graph city;
std::vector<Shelter> shelters = {Shelter(1, "Stadium_Shelter", 10, 300),
                                 Shelter(2, "Hall_Shelter", 11, 150),
                                 Shelter(3, "Camp_Shelter", 12, 200)};
std::vector<EvacuationRequest> requests;
std::vector<RoadReport> reports;

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
    if (city.loadFromFile(MAP_FILE))
        std::cout << "Loaded " << city.locationCount() << " locations and " << city.roadCount()
                  << " roads from " << MAP_FILE << ".\n";
}

int main() {
    loadCity();

    while (true) {
        std::cout << "\n===== RescueNet =====\n"
                  << "1. Show locations\n2. Show roads\n3. Show shelters\n"
                  << "4. Add evacuation request\n5. Show evacuation requests\n"
                  << "6. Add road report\n7. Show road reports\n"
                  << "8. Reload city map\n9. Show map (adjacency list)\n"
                  << "10. Find shortest route\n11. Block road\n12. Restore road\n0. Exit\n";
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
            for (const Shelter& s : shelters)
                std::cout << "  Shelter " << s.getId() << ": " << s.getName() << " at location "
                          << s.getLocationId() << ", " << s.getOccupancy() << "/" << s.getCapacity() << "\n";
        } else if (choice == 4) {
            int id = static_cast<int>(requests.size()) + 1;
            int location = readInt("Location ID: ");
            int people = readInt("Number of people: ");
            int danger = readInt("Danger level (1-5): ");
            requests.push_back(EvacuationRequest(id, location, people, danger, false, id));
            std::cout << "Request " << id << " added.\n";
        } else if (choice == 5) {
            for (const EvacuationRequest& r : requests)
                std::cout << "  Request " << r.getId() << ": location " << r.getLocationId() << ", "
                          << r.getGroupSize() << " people, danger " << r.getDangerLevel() << "\n";
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
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
    std::cout << "Goodbye.\n";
}
