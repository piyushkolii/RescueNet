#pragma once

#include <string>

class Shelter {
public:
    Shelter();
    Shelter(int id, const std::string& name, int locationId, int capacity);

    int getId() const;
    const std::string& getName() const;
    int getLocationId() const;
    int getCapacity() const;
    int getOccupancy() const;
    int getRemainingCapacity() const;
    bool isFull() const;
    bool canAccommodate(int groupSize) const;

    bool acceptEvacuees(int groupSize);

private:
    int id;
    std::string name;
    int locationId;
    int capacity;
    int occupancy;
};