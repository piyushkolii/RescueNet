#include "MinHeap.h"

#include <stdexcept>
#include <utility>

MinHeap::MinHeap() : data() {}

void MinHeap::push(double distance, int node) {
    data.push_back({distance, node});
    siftUp(static_cast<int>(data.size()) - 1);
}

MinHeap::Entry MinHeap::pop() {
    if (data.empty()) throw std::out_of_range("pop from empty MinHeap");
    Entry smallest = data[0];
    data[0] = data.back();
    data.pop_back();
    if (!data.empty()) siftDown(0);
    return smallest;
}

const MinHeap::Entry& MinHeap::top() const {
    if (data.empty()) throw std::out_of_range("top of empty MinHeap");
    return data[0];
}

bool MinHeap::empty() const { return data.empty(); }
int MinHeap::size() const { return static_cast<int>(data.size()); }

void MinHeap::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (data[parent].distance <= data[i].distance) break;
        std::swap(data[parent], data[i]);
        i = parent;
    }
}

void MinHeap::siftDown(int i) {
    int n = static_cast<int>(data.size());
    while (true) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && data[left].distance < data[smallest].distance) smallest = left;
        if (right < n && data[right].distance < data[smallest].distance) smallest = right;
        if (smallest == i) break;
        std::swap(data[i], data[smallest]);
        i = smallest;
    }
}
