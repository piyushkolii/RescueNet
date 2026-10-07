#include "Shelter.h"

Shelter::Shelter() : id(0), name(""), locationId(0), capacity(0), occupancy(0) {}

Shelter::Shelter(int id_, const std::string& name_, int locationId_, int capacity_)
    : id(id_), name(name_), locationId(locationId_),
      capacity(capacity_ < 0 ? 0 : capacity_), occupancy(0) {}

int Shelter::getId() const { return id; }
const std::string& Shelter::getName() const { return name; }
int Shelter::getLocationId() const { return locationId; }
int Shelter::getCapacity() const { return capacity; }
int Shelter::getOccupancy() const { return occupancy; }
int Shelter::getRemainingCapacity() const { return capacity - occupancy; }
bool Shelter::isFull() const { return occupancy >= capacity; }

bool Shelter::canAccommodate(int groupSize) const {
    return groupSize > 0 && groupSize <= getRemainingCapacity();
}

bool Shelter::acceptEvacuees(int groupSize) {
    if (!canAccommodate(groupSize)) return false;
    occupancy += groupSize;
    return true;
}