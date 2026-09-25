#include "EvacuationRequest.h"

EvacuationRequest::EvacuationRequest()
    : id(0), locationId(0), peopleCount(0),
      dangerLevel(0), arrivalTime(0) {}

EvacuationRequest::EvacuationRequest(int id_, int locationId_,
                                     int peopleCount_, int dangerLevel_,
                                     int arrivalTime_)
    : id(id_), locationId(locationId_), peopleCount(peopleCount_),
      dangerLevel(dangerLevel_), arrivalTime(arrivalTime_) {}

int EvacuationRequest::getId() const {
    return id;
}

int EvacuationRequest::getLocationId() const {
    return locationId;
}

int EvacuationRequest::getPeopleCount() const {
    return peopleCount;
}

int EvacuationRequest::getDangerLevel() const {
    return dangerLevel;
}

int EvacuationRequest::getArrivalTime() const {
    return arrivalTime;
}

bool EvacuationRequest::hasHigherPriorityThan(
    const EvacuationRequest& other) const {

    if (dangerLevel != other.dangerLevel)
        return dangerLevel > other.dangerLevel;

    return arrivalTime < other.arrivalTime;
}