#pragma once

#include <string>

// A node in the road network: an area or a shelter site.
class Location {
public:
    Location();
    Location(int id, const std::string& name, const std::string& type);

    int getId() const;
    const std::string& getName() const;
    const std::string& getType() const; // "AREA" or "SHELTER"

private:
    int id;
    std::string name;
    std::string type;
};
