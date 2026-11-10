#pragma once

// FIFO queue (singly linked list) of road report IDs waiting for an operator.
// Reports are reviewed strictly in the order they were submitted.
class ReportQueue {
public:
    ReportQueue();
    ~ReportQueue();
    ReportQueue(const ReportQueue&) = delete;
    ReportQueue& operator=(const ReportQueue&) = delete;

    void enqueue(int reportId);
    int dequeue();       // throws if empty
    int front() const;   // throws if empty
    bool empty() const;
    int size() const;

private:
    struct Node {
        int reportId;
        Node* next;
    };
    Node* head;
    Node* tail;
    int count;
};
