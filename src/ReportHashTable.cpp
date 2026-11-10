#include "ReportHashTable.h"

#include <algorithm>

ReportHashTable::ReportHashTable(int bucketCount)
    : buckets(bucketCount < 1 ? 1 : bucketCount, nullptr), count(0) {}

ReportHashTable::~ReportHashTable() {
    for (Node* head : buckets) {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
}

int ReportHashTable::hash(int key) const {
    int n = static_cast<int>(buckets.size());
    int h = key % n;
    return h < 0 ? h + n : h;
}

bool ReportHashTable::insert(const RoadReport& report) {
    if (find(report.getId())) return false;
    int b = hash(report.getId());
    buckets[b] = new Node{report, buckets[b]};
    ++count;
    return true;
}

RoadReport* ReportHashTable::find(int reportId) {
    for (Node* n = buckets[hash(reportId)]; n; n = n->next)
        if (n->report.getId() == reportId) return &n->report;
    return nullptr;
}

const RoadReport* ReportHashTable::find(int reportId) const {
    for (const Node* n = buckets[hash(reportId)]; n; n = n->next)
        if (n->report.getId() == reportId) return &n->report;
    return nullptr;
}

bool ReportHashTable::remove(int reportId) {
    Node** link = &buckets[hash(reportId)];
    while (*link) {
        if ((*link)->report.getId() == reportId) {
            Node* dead = *link;
            *link = dead->next;
            delete dead;
            --count;
            return true;
        }
        link = &(*link)->next;
    }
    return false;
}

int ReportHashTable::size() const { return count; }

std::vector<const RoadReport*> ReportHashTable::all() const {
    std::vector<const RoadReport*> out;
    for (const Node* head : buckets)
        for (const Node* n = head; n; n = n->next) out.push_back(&n->report);
    std::sort(out.begin(), out.end(),
              [](const RoadReport* a, const RoadReport* b) { return a->getId() < b->getId(); });
    return out;
}
