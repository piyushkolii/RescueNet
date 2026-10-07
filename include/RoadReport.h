#pragma once

#include <string>

enum class ReportStatus { PENDING, VERIFIED, REJECTED };

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
