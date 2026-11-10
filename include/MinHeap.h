#pragma once

#include <vector>


class MinHeap {
public:
    struct Entry {
        double distance;
        int node;
    };

    MinHeap();

    void push(double distance, int node);
    Entry pop();         
    const Entry& top() const;
    bool empty() const;
    int size() const;

private:
    std::vector<Entry> data;

    void siftUp(int i);
    void siftDown(int i);
};
