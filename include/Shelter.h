#pragma once

#include <string>

// A shelter placed at a Location in the road network. Capacity and occupancy
// are private and occupancy only changes through acceptEvacuees().
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

    // Admits the whole group if there is room; returns false and changes
    // nothing otherwise (each request moves as one group).
    bool acceptEvacuees(int groupSize);

    // Gives back space when a group is moved to another shelter (rerouting).
    void releaseEvacuees(int groupSize);

private:
    int id;
    std::string name;
    int locationId;
    int capacity;
    int occupancy;
};
