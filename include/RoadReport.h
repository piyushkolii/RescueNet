#pragma once

#include <string>

// PENDING until an operator reviews it; only VERIFIED reports may change the graph.
enum class ReportStatus { PENDING, VERIFIED, REJECTED };

// A citizen/volunteer report that a road is blocked (or cleared again).
// It never touches the graph by itself; the operator's decision is final.
class RoadReport {
public:
    RoadReport();
    RoadReport(int id, int roadId, bool reportsBlocked,
               const std::string& reporter, const std::string& description);

    int getId() const;
    int getRoadId() const;
    bool reportsBlocked() const; // true = "road is blocked", false = "road is clear again"
    const std::string& getReporter() const;
    const std::string& getDescription() const;
    ReportStatus getStatus() const;
    bool isPending() const;

    // Only a PENDING report can be decided; returns false if already decided.
    bool verify();
    bool reject();

private:
    int id;
    int roadId;
    bool blocked;
    std::string reporter;
    std::string description;
    ReportStatus status;
};
