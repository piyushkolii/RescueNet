#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#include "RescueNetSystem.h"

namespace {

// Keeps asking until the user types a whole number in [lo, hi].
int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value && value >= lo && value <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof()) std::exit(0);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line.empty() ? "-" : line;
}

void printMenu() {
    std::cout << "\n========== RescueNet ==========\n"
              << " 1. Show city map\n"
              << " 2. Show shelters\n"
              << " 3. Find shortest route\n"
              << " 4. Add evacuation request\n"
              << " 5. Show pending requests\n"
              << " 6. Process next request\n"
              << " 7. Process all requests\n"
              << " 8. Show processed requests\n"
              << " 9. Mark request evacuated\n"
              << "10. Submit road report\n"
              << "11. Review next road report\n"
              << "12. Find report by ID\n"
              << "13. Show all road reports\n"
              << " 0. Exit\n";
}

void reviewNextReport(RescueNetSystem& sys) {
    const RoadReport* r = sys.peekNextReport();
    if (!r) {
        std::cout << "No reports waiting for review.\n";
        return;
    }
    std::cout << "Report " << r->getId() << ": road " << r->getRoadId() << " is "
              << (r->reportsBlocked() ? "BLOCKED" : "CLEAR again")
              << " (by " << r->getReporter() << ": " << r->getDescription() << ")\n";
    int choice = readInt("1 = verify, 2 = reject: ", 1, 2);
    sys.decideNextReport(choice == 1);
}

}  // namespace

int main(int argc, char* argv[]) {
    std::string mapPath = argc > 1 ? argv[1] : "data/city.txt";

    RescueNetSystem sys;
    if (!sys.loadCity(mapPath)) {
        std::cout << "Failed to load city map. Usage: rescuenet [map-file]\n";
        return 1;
    }
    std::cout << "Loaded " << sys.getGraph().locationCount() << " locations, "
              << sys.getGraph().roadCount() << " roads, "
              << sys.getShelters().count() << " shelters from " << mapPath << "\n";

    while (true) {
        printMenu();
        int choice = readInt("Choose: ", 0, 13);
        std::cout << "\n";

        switch (choice) {
            case 0:
                std::cout << "Goodbye.\n";
                return 0;
            case 1:
                sys.printMap();
                break;
            case 2:
                sys.printShelters();
                break;
            case 3: {
                int from = readInt("From location ID: ", 0, 1000000);
                int to = readInt("To location ID: ", 0, 1000000);
                if (!sys.getGraph().getLocation(from) || !sys.getGraph().getLocation(to)) {
                    std::cout << "Unknown location.\n";
                    break;
                }
                sys.findRoute(from, to).print();
                break;
            }
            case 4: {
                int loc = readInt("Location ID: ", 0, 1000000);
                int group = readInt("Group size: ", 1, 100000);
                int danger = readInt("Danger level (1-5): ", 1, 5);
                bool vulnerable = readInt("Vulnerable people? (1 = yes, 0 = no): ", 0, 1) == 1;
                int id = sys.addRequest(loc, group, danger, vulnerable);
                if (id == -1) std::cout << "Invalid request (unknown location).\n";
                else std::cout << "Request " << id << " added to the priority queue.\n";
                break;
            }
            case 5:
                sys.printPendingRequests();
                break;
            case 6:
                if (!sys.processNextRequest()) std::cout << "No pending requests.\n";
                break;
            case 7:
                std::cout << sys.processAllRequests() << " request(s) processed.\n";
                break;
            case 8:
                sys.printAssignments();
                break;
            case 9: {
                int id = readInt("Request ID: ", 1, 1000000);
                std::cout << (sys.markEvacuated(id) ? "Marked as evacuated.\n"
                                                    : "No assigned request with that ID.\n");
                break;
            }
            case 10: {
                int road = readInt("Road ID: ", 0, 1000000);
                bool blocked = readInt("1 = road blocked, 2 = road cleared: ", 1, 2) == 1;
                std::string reporter = readLine("Reporter name: ");
                std::string desc = readLine("Description: ");
                int id = sys.submitReport(road, blocked, reporter, desc);
                if (id == -1) std::cout << "Unknown road.\n";
                else std::cout << "Report " << id << " queued for review ("
                               << sys.pendingReportCount() << " waiting).\n";
                break;
            }
            case 11:
                reviewNextReport(sys);
                break;
            case 12: {
                int id = readInt("Report ID: ", 1, 1000000);
                const RoadReport* r = sys.findReport(id);
                if (!r) std::cout << "No report with that ID.\n";
                else std::cout << "Report " << r->getId() << ": road " << r->getRoadId()
                               << (r->reportsBlocked() ? " BLOCKED" : " CLEARED")
                               << ", by " << r->getReporter() << " - " << r->getDescription()
                               << (r->isPending() ? " [pending]" : " [decided]") << "\n";
                break;
            }
            case 13:
                sys.printReports();
                break;
        }
    }
}
