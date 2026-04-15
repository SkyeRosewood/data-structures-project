#ifndef STACK_H
#define STACK_H
#include <iostream>
using namespace std;

//node structure
struct Node{
    int data;
    Node* next;
};

//stack class using singly linked list
class Stack{
    private:
        Node* top;

    public:
        Stack();          //constructor
        ~Stack();         //destructor

        void push(int data); //needed class functions
        void pop();
        int peek();
        bool isEmpty();
        void printList();
};

#endif