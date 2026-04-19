#include "stack.h"
using namespace std;

StackNode::StackNode(int data) : data(data), next(nullptr) {}

Stack::Stack() : top(nullptr), count(0) {}

Stack::~Stack() {
    while (top) { StackNode* t=top; top=top->next; delete t; }
}

void Stack::push(int data) {
    StackNode* node=new StackNode(data);
    node->next=top; top=node; ++count;
}

int Stack::pop() {
    if (isEmpty()) throw underflow_error("Stack is empty");
    StackNode* t=top; int v=t->data;
    top=top->next; delete t; --count; return v;
}

int Stack::peek() const { if(isEmpty()) throw underflow_error("Stack is empty"); return top->data; }
bool Stack::isEmpty() const { return top==nullptr; }
int Stack::size() const { return count; }