#include "Location.h"

Location::Location() : id(0), name(""), type("AREA") {}

Location::Location(int id_, const std::string& name_, const std::string& type_)
    : id(id_), name(name_), type(type_) {}

int Location::getId() const { return id; }
const std::string& Location::getName() const { return name; }
const std::string& Location::getType() const { return type; }
