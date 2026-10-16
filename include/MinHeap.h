#pragma once

#include <vector>

// Array-based binary min-heap of (distance, location index) pairs, used by
// Dijkstra to always expand the closest unvisited location next.
class MinHeap {
public:
    struct Entry {
        double distance;
        int node;
    };

    MinHeap();

    void push(double distance, int node);
    Entry pop();          // removes and returns the smallest entry
    const Entry& top() const;
    bool empty() const;
    int size() const;

private:
    std::vector<Entry> data;

    void siftUp(int i);
    void siftDown(int i);
};
