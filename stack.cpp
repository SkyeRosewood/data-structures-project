#include "stack.h"
using namespace std;

StackNode::StackNode(int data) : data(data), next(nullptr) {}

// Initializes an empty Last-In-First-Out (LIFO) structure
Stack::Stack() : top(nullptr), count(0) {}

//destructor for the stack
Stack::~Stack() {
    while (top) { 
        StackNode* t = top; 
        top = top->next; 
        delete t; 
    }
}

// inserts to the head of the list
void Stack::push(int data) {
    StackNode* node = new StackNode(data);
    node->next = top; 
    top = node; 
    ++count;
}

// Removes the head node and returns its value in O(1) time
int Stack::pop() {
    if (isEmpty()) throw underflow_error("Stack is empty");
    
    StackNode* t = top; 
    int v = t->data;
    top = top->next; 
    
    delete t; 
    --count; 
    return v;
}

// checks the stack's state
int Stack::peek() const { 
    if(isEmpty()) throw underflow_error("Stack is empty"); 
    return top->data; 
}

bool Stack::isEmpty() const { return top == nullptr; }

int Stack::size() const { return count; }