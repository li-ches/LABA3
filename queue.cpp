#include "queue.h"
#include <iostream>
#include <fstream>

using namespace std;

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() {
    while (head) {
        QNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void Queue::push(string val) {
    QNode* newNode = new QNode{val, nullptr};
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

string Queue::pop() {
    if (!head) return "[QUEUE_EMPTY]";
    QNode* temp = head;
    string val = head->value;
    head = head->next;
    if (!head) tail = nullptr;
    delete temp;
    return val;
}

string Queue::peek() const {
    if (!head) return "[QUEUE_EMPTY]";
    return head->value;
}

void Queue::print() const {
    if (!head) {
        cout << "Очередь пуста." << endl;
        return;
    }
    cout << "Очередь: ";
    QNode* curr = head;
    while (curr) {
        cout << curr->value;
        if (curr->next) cout << " -> ";
        curr = curr->next;
    }
    cout << endl;
}

bool Queue::isEmpty() const {
    return head == nullptr;
}
