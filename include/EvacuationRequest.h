#pragma once

#include <string>

class EvacuationRequest {
public:
    EvacuationRequest();
    EvacuationRequest(int id, int locationId, int peopleCount,
                      int dangerLevel, int arrivalTime);

    int getId() const;
    int getLocationId() const;
    int getPeopleCount() const;
    int getDangerLevel() const;
    int getArrivalTime() const;

    bool hasHigherPriorityThan(const EvacuationRequest& other) const;

private:
    int id;
    int locationId;
    int peopleCount;
    int dangerLevel;
    int arrivalTime;
};