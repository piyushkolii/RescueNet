#include "RoadReport.h"

RoadReport::RoadReport()
    : id(0), roadId(0), blocked(true), reporter(""), description(""),
      status(ReportStatus::PENDING) {}

RoadReport::RoadReport(int id_, int roadId_, bool reportsBlocked_,
                       const std::string& reporter_, const std::string& description_)
    : id(id_), roadId(roadId_), blocked(reportsBlocked_), reporter(reporter_),
      description(description_), status(ReportStatus::PENDING) {}

int RoadReport::getId() const { return id; }
int RoadReport::getRoadId() const { return roadId; }
bool RoadReport::reportsBlocked() const { return blocked; }
const std::string& RoadReport::getReporter() const { return reporter; }
const std::string& RoadReport::getDescription() const { return description; }
ReportStatus RoadReport::getStatus() const { return status; }
bool RoadReport::isPending() const { return status == ReportStatus::PENDING; }

bool RoadReport::verify() {
    if (!isPending()) return false;
    status = ReportStatus::VERIFIED;
    return true;
}

bool RoadReport::reject() {
    if (!isPending()) return false;
    status = ReportStatus::REJECTED;
    return true;
}
