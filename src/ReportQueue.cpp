#include "ReportQueue.h"

#include <stdexcept>

ReportQueue::ReportQueue() : head(nullptr), tail(nullptr), count(0) {}

ReportQueue::~ReportQueue() {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

void ReportQueue::enqueue(int reportId) {
    Node* node = new Node{reportId, nullptr};
    if (tail) tail->next = node;
    else head = node;
    tail = node;
    ++count;
}

int ReportQueue::dequeue() {
    if (!head) throw std::out_of_range("dequeue from empty ReportQueue");
    Node* old = head;
    int id = old->reportId;
    head = head->next;
    if (!head) tail = nullptr;
    delete old;
    --count;
    return id;
}

int ReportQueue::front() const {
    if (!head) throw std::out_of_range("front of empty ReportQueue");
    return head->reportId;
}

bool ReportQueue::empty() const { return head == nullptr; }
int ReportQueue::size() const { return count; }
