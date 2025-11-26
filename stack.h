#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class StackSerializer;

class Stack {
private:
    class SNode {
    public:
        string value;
        SNode* next;
    };

    SNode* top;

public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    Stack(Stack&&) = delete;
    Stack& operator=(Stack&&) = delete;

    void push(string val);
    string peek() const;
    string pop();
    void readStack() const;
    bool isEmpty() const;

    SNode* getTop_test() const { return top; }

private:
    friend class StackSerializer;
};

#endif