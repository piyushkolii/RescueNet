#pragma once

#include <vector>

#include "RoadReport.h"

// Hash table (separate chaining) holding every road report keyed by its ID,
// so any report can be looked up in O(1) average time.
class ReportHashTable {
public:
    explicit ReportHashTable(int bucketCount = 31);
    ~ReportHashTable();
    ReportHashTable(const ReportHashTable&) = delete;
    ReportHashTable& operator=(const ReportHashTable&) = delete;

    bool insert(const RoadReport& report);   // false if the ID already exists
    RoadReport* find(int reportId);
    const RoadReport* find(int reportId) const;
    bool remove(int reportId);
    int size() const;

    // All reports sorted by ID (for display).
    std::vector<const RoadReport*> all() const;

private:
    struct Node {
        RoadReport report;
        Node* next;
    };
    std::vector<Node*> buckets;
    int count;

    int hash(int key) const;
};
