#include "stack.h"
#include <iostream>
#include <fstream>

using namespace std;

Stack::Stack() : top(nullptr) {}

Stack::~Stack() {
    while (top) {
        SNode* temp = top;
        top = top->next;
        delete temp;
    }
}

void Stack::push(string val) {
    SNode* newNode = new SNode{val, top};
    top = newNode;
}

string Stack::peek() const {
    if (!top) return "[STACK_EMPTY]";
    return top->value;
}

string Stack::pop() {
    if (!top) return "[STACK_EMPTY]";
    SNode* temp = top;
    string val = top->value;
    top = top->next;
    delete temp;
    return val;
}

void Stack::readStack() const {
    if (!top) {
        cout << "Стек пуст." << endl;
        return;
    }
    cout << "Стек (верх -> низ): ";
    SNode* curr = top;
    while (curr) {
        cout << curr->value;
        if (curr->next) cout << " -> ";
        curr = curr->next;
    }
    cout << endl;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}
