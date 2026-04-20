#include "stack.h"

//constructor
Stack::Stack(){
    top=nullptr;
}
//destructor
Stack::~Stack(){
    while(!isEmpty()){
        pop();
    }
}

//push function
void Stack::push(int data){
    Node* temp=new Node();
    if(!temp){
        cout<<"\nStack Overflow";
        exit(1);
    }
    temp->data=data;
    temp->next=top;
    top=temp;
}

//check if empty
bool Stack::isEmpty(){
    return top==nullptr;
}

//peek function
int Stack::peek(){
    if(!isEmpty()){
        return top->data;
    }
    else{
        cout<<"\nStack is empty";
        exit(1);
    }
}

//pop function
int Stack::pop(){
    if(isEmpty()){
        cout<<"\nStack Underflow"<<endl;
        exit(1);
    }
    Node* temp=top;
    int item=top->data;
    top=top->next;
    delete temp;
    return item;
}

//print stack
void Stack::printList(){
    if(isEmpty()){
        cout<<"\nStack is empty";
        return;
    }
    Node* temp=top;
    while(temp!=nullptr){
        cout<<temp->data<<" -> ";
        temp=temp->next;
    }
    cout<<"NULL"<<endl;
}