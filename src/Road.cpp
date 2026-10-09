#include "Road.h"

Road::Road() : id(0), fromId(0), toId(0), weight(0.0), status(RoadStatus::OPEN) {}

Road::Road(int id_, int fromId_, int toId_, double weight_)
    : id(id_), fromId(fromId_), toId(toId_), weight(weight_), status(RoadStatus::OPEN) {}

int Road::getId() const { return id; }
int Road::getFrom() const { return fromId; }
int Road::getTo() const { return toId; }
double Road::getWeight() const { return weight; }
RoadStatus Road::getStatus() const { return status; }
bool Road::isOpen() const { return status == RoadStatus::OPEN; }

void Road::block() { status = RoadStatus::BLOCKED; }
void Road::restore() { status = RoadStatus::OPEN; }
