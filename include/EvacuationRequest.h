#pragma once

// PENDING until a shelter is chosen, ASSIGNED once one is, EVACUATED when the
// group reaches it, FAILED when no shelter or route is available.
enum class RequestStatus { PENDING, ASSIGNED, EVACUATED, FAILED };

// A request to move one group from a Location to a shelter. Priority is fixed
// when the request is entered; ties are broken by arrival order.
class EvacuationRequest {
public:
    EvacuationRequest();
    EvacuationRequest(int id, int locationId, int groupSize, int dangerLevel,
                      bool vulnerable, int arrivalOrder);

    int getId() const;
    int getLocationId() const;
    int getGroupSize() const;
    int getDangerLevel() const;   // 1 (low) to 5 (critical)
    bool isVulnerable() const;    // elderly, children, injured, etc.
    int getArrivalOrder() const;
    int getPriority() const;      // higher = more urgent
    RequestStatus getStatus() const;
    int getAssignedShelterId() const; // -1 if none

    // True if this request should be served before `other`.
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
