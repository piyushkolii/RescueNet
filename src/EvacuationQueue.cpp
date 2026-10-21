#include "EvacuationQueue.h"

#include <iostream>
#include <stdexcept>
#include <utility>

EvacuationQueue::EvacuationQueue() : heap() {}

void EvacuationQueue::push(const EvacuationRequest& request) {
    heap.push_back(request);
    siftUp(static_cast<int>(heap.size()) - 1);
}

EvacuationRequest EvacuationQueue::pop() {
    if (heap.empty()) throw std::out_of_range("pop from empty EvacuationQueue");
    EvacuationRequest best = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) siftDown(0);
    return best;
}

const EvacuationRequest& EvacuationQueue::top() const {
    if (heap.empty()) throw std::out_of_range("top of empty EvacuationQueue");
    return heap[0];
}

bool EvacuationQueue::empty() const { return heap.empty(); }
int EvacuationQueue::size() const { return static_cast<int>(heap.size()); }

void EvacuationQueue::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!heap[i].hasHigherPriorityThan(heap[parent])) break;
        std::swap(heap[i], heap[parent]);
        i = parent;
    }
}

void EvacuationQueue::siftDown(int i) {
    int n = static_cast<int>(heap.size());
    while (true) {
        int best = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && heap[left].hasHigherPriorityThan(heap[best])) best = left;
        if (right < n && heap[right].hasHigherPriorityThan(heap[best])) best = right;
        if (best == i) break;
        std::swap(heap[i], heap[best]);
        i = best;
    }
}

void EvacuationQueue::print() const {
    if (heap.empty()) {
        std::cout << "No pending evacuation requests.\n";
        return;
    }
    EvacuationQueue copy = *this;   // drain a copy so the real queue is untouched
    int rank = 1;
    while (!copy.empty()) {
        EvacuationRequest r = copy.pop();
        std::cout << "  " << rank++ << ". Request " << r.getId()
                  << " | location " << r.getLocationId()
                  << " | group " << r.getGroupSize()
                  << " | danger " << r.getDangerLevel()
                  << (r.isVulnerable() ? " | vulnerable" : "")
                  << " | priority " << r.getPriority() << "\n";
    }
}
