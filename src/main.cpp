#include <iostream>
#include <string>
#include <vector>

#include "EvacuationRequest.h"
#include "Location.h"
#include "Road.h"
#include "RoadReport.h"
#include "Shelter.h"

std::vector<Location> locations = {
    Location(1, "Central_Market", "AREA"), Location(2, "Railway_Station", "AREA"),
    Location(3, "Old_Town", "AREA"),       Location(4, "City_Stadium", "SHELTER")};
std::vector<Road> roads = {Road(1, 1, 2, 4), Road(2, 1, 3, 3), Road(3, 2, 4, 7), Road(4, 3, 4, 5)};
std::vector<Shelter> shelters = {Shelter(1, "Stadium_Shelter", 4, 300)};
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

int main() {
    while (true) {
        std::cout << "\n===== RescueNet =====\n"
                  << "1. Show locations\n2. Show roads\n3. Show shelters\n"
                  << "4. Add evacuation request\n5. Show evacuation requests\n"
                  << "6. Add road report\n7. Show road reports\n0. Exit\n";
        int choice = readInt("Choose: ");

        if (choice == 0) break;
        if (choice == 1) {
            for (const Location& l : locations)
                std::cout << "  [" << l.getId() << "] " << l.getName() << " (" << l.getType() << ")\n";
        } else if (choice == 2) {
            for (const Road& r : roads)
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
            requests.push_back(EvacuationRequest(id, location, people, danger, id));
            std::cout << "Request " << id << " added.\n";
        } else if (choice == 5) {
            for (const EvacuationRequest& r : requests)
                std::cout << "  Request " << r.getId() << ": location " << r.getLocationId() << ", "
                          << r.getPeopleCount() << " people, danger " << r.getDangerLevel() << "\n";
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
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
    std::cout << "Goodbye.\n";
}
