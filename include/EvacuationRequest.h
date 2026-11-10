#pragma once

enum class RequestStatus { PENDING, ASSIGNED, EVACUATED, FAILED };

class EvacuationRequest {
public:
    EvacuationRequest();
    EvacuationRequest(int id, int locationId, int groupSize, int dangerLevel,
                      bool vulnerable, int arrivalOrder);

    int getId() const;
    int getLocationId() const;
    int getGroupSize() const;
    int getDangerLevel() const; 
    bool isVulnerable() const;    
    int getArrivalOrder() const;
    int getPriority() const;  
    RequestStatus getStatus() const;
    int getAssignedShelterId() const;

    bool hasHigherPriorityThan(const EvacuationRequest& other) const;

    void assignShelter(int shelterId);
    void markEvacuated();
    void markFailed();

private:
    int id;
    int locationId;
    int groupSize;
    int dangerLevel;
    bool vulnerable;
    int arrivalOrder;
    RequestStatus status;
    int assignedShelterId;
};
