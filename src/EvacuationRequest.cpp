#include "EvacuationRequest.h"

EvacuationRequest::EvacuationRequest()
    : id(0), locationId(0), groupSize(0), dangerLevel(1), vulnerable(false),
      arrivalOrder(0), status(RequestStatus::PENDING), assignedShelterId(-1) {}

EvacuationRequest::EvacuationRequest(int id_, int locationId_, int groupSize_,
                                     int dangerLevel_, bool vulnerable_, int arrivalOrder_)
    : id(id_), locationId(locationId_), groupSize(groupSize_),
      dangerLevel(dangerLevel_ < 1 ? 1 : (dangerLevel_ > 5 ? 5 : dangerLevel_)),
      vulnerable(vulnerable_), arrivalOrder(arrivalOrder_),
      status(RequestStatus::PENDING), assignedShelterId(-1) {}

int EvacuationRequest::getId() const { return id; }
int EvacuationRequest::getLocationId() const { return locationId; }
int EvacuationRequest::getGroupSize() const { return groupSize; }
int EvacuationRequest::getDangerLevel() const { return dangerLevel; }
bool EvacuationRequest::isVulnerable() const { return vulnerable; }
int EvacuationRequest::getArrivalOrder() const { return arrivalOrder; }
RequestStatus EvacuationRequest::getStatus() const { return status; }
int EvacuationRequest::getAssignedShelterId() const { return assignedShelterId; }

// Danger level dominates, then vulnerability, then group size.
int EvacuationRequest::getPriority() const {
    return dangerLevel * 1000 + (vulnerable ? 500 : 0) + groupSize;
}

bool EvacuationRequest::hasHigherPriorityThan(const EvacuationRequest& other) const {
    if (getPriority() != other.getPriority()) return getPriority() > other.getPriority();
    return arrivalOrder < other.arrivalOrder;
}

void EvacuationRequest::assignShelter(int shelterId) {
    assignedShelterId = shelterId;
    status = RequestStatus::ASSIGNED;
}

void EvacuationRequest::markEvacuated() { status = RequestStatus::EVACUATED; }
void EvacuationRequest::markFailed() { status = RequestStatus::FAILED; }
