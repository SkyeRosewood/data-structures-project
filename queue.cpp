#include "queue.h"
using namespace std;

QueueNode::QueueNode(int value) : value(value), next(nullptr) {}

// Initializes an empty FIFO structure
Queue::Queue() : head(nullptr), tail(nullptr), count(0) {}

// destructor for the queue class and its nodes
Queue::~Queue() {
    while (head) { 
        QueueNode* t = head; 
        head = head->next; 
        delete t; 
    }
}

// append a new node to the tail of the list
void Queue::enqueue(int value) {
    QueueNode* node = new QueueNode(value);
    
    // If the queue has items, link the old tail to the new node
    // Otherwise new node becomes the head too
    if (tail) tail->next = node; 
    else head = node;
    
    tail = node; 
    ++count;
}

// Removes the head node and returns its value 
int Queue::dequeue() {
    if (isEmpty()) throw underflow_error("Queue is empty");
    
    QueueNode* t = head; 
    int v = t->value;
    head = head->next; 
    
    // Critical edge case: if we just removed the very last item, 
    // the tail pointer must also be reset to null to avoid dangling pointers
    if(!head) tail = nullptr;
    
    delete t; 
    --count; 
    return v;
}

// Read-only utility methods for checking the queue's state
int Queue::front() const { 
    if(isEmpty()) throw underflow_error("Queue is empty"); 
    return head->value; 
}

bool Queue::isEmpty() const { return count == 0; }

int Queue::size() const { return count; }