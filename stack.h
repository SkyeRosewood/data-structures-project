#ifndef STACK_H
#define STACK_H

#include <stdexcept>
//defines individual elements of the stack's linked list
struct StackNode {
    int data;
    StackNode* next;
    explicit StackNode(int data);
};
//LIFO data structure
class Stack {
public:
    Stack();
    ~Stack();
    //operations
    void push(int data);
    int pop();
    int peek() const;
    //checks the stack's state
    bool isEmpty() const;
    int size() const;

private:
    StackNode* top;//makes popping quicker
    int count;//size
};

#endif