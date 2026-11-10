#pragma once

#include <string>

class Location {
public:
    Location();
    Location(int id, const std::string& name, const std::string& type);

    int getId() const;
    const std::string& getName() const;
    const std::string& getType() const; 

private:
    int id;
    std::string name;
    std::string type;
};
