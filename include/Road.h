#pragma once

enum class RoadStatus { OPEN, BLOCKED };

class Road {
public:
    Road();
    Road(int id, int fromId, int toId, double weight);

    int getId() const;
    int getFrom() const;
    int getTo() const;
    double getWeight() const;
    RoadStatus getStatus() const;
    bool isOpen() const;

    void block();
    void restore();

private:
    int id;
    int fromId;
    int toId;
    double weight;
    RoadStatus status;
};
