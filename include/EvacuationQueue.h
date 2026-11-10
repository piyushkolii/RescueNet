#pragma once

#include <vector>

#include "EvacuationRequest.h"

class EvacuationQueue {
public:
    EvacuationQueue();

    void push(const EvacuationRequest& request);
    EvacuationRequest pop();             // removes and returns the most urgent
    const EvacuationRequest& top() const;
    bool empty() const;
    int size() const;

    // Prints pending requests in the order they would be served.
    void print() const;

private:
    std::vector<EvacuationRequest> heap;

    void siftUp(int i);
    void siftDown(int i);
};
