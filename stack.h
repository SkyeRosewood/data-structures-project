#ifndef STACK_H
#define STACK_H

#include <stdexcept>

struct StackNode {
    int data;
    StackNode* next;
    explicit StackNode(int data);
};

class Stack {
public:
    Stack();
    ~Stack();

    void push(int data);
    int pop();
    int peek() const;
    bool isEmpty() const;
    int size() const;

private:
    StackNode* top;
    int count;
};

#endif