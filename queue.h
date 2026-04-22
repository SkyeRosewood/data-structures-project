#ifndef QUEUE_H
#define QUEUE_H
#include <stdexcept>

// Represents a single element in the queue's underlying linked list
struct QueueNode {
    int value;
    QueueNode* next;
    explicit QueueNode(int value);
};

//FIFO data structure
class Queue {
public:
    Queue();
    ~Queue();

    //add to the back, remove/view from the front
    void enqueue(int value);
    int dequeue();
    int front() const;
    
    //check the queue's current state
    bool isEmpty() const;
    int size() const;

private:
    // Internal tracking pointers
    QueueNode* head;
    QueueNode* tail;
    
    //size
    int count;
};

#endif