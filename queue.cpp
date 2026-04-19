#include "queue.h"
using namespace std;

QueueNode::QueueNode(int value) : value(value), next(nullptr) {}

Queue::Queue() : head(nullptr), tail(nullptr), count(0) {}

Queue::~Queue() {
    while (head) { QueueNode* t=head; head=head->next; delete t; }
}

void Queue::enqueue(int value) {
    QueueNode* node = new QueueNode(value);
    if (tail) tail->next=node; else head=node;
    tail=node; ++count;
}

int Queue::dequeue() {
    if (isEmpty()) throw underflow_error("Queue is empty");
    QueueNode* t=head; int v=t->value;
    head=head->next; if(!head) tail=nullptr;
    delete t; --count; return v;
}

int Queue::front() const { if(isEmpty()) throw underflow_error("Queue is empty"); return head->value; }
bool Queue::isEmpty() const { return count==0; }
int Queue::size() const { return count; }