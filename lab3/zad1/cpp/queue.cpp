#include <iostream>
#include <fstream>
#include "node.h"
#include "queue.h"

using namespace std;

Queue::Queue() {
    front = nullptr;
    rear = nullptr;
}

Queue::~Queue() {
    while (front != nullptr) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
}

void Queue::enqueue(const string& val) {
    auto *newNode = new QueueNode(val);

    if (front == nullptr) {
        front = newNode;
        rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

void Queue::dequeue() {
    if (front == nullptr) {
        throw runtime_error("error");
    }
    QueueNode* temp = front;
    front = front->next;
    
    if (front == nullptr) {
        rear = nullptr;
    }
    delete temp;
}

void Queue::print() const {
    if (front == nullptr) {
        cout << "error" << '\n';
        return;
    }
    QueueNode* curr = front;
    cout << "очередь: ";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << '\n';
}

void Queue::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    QueueNode* curr = front;
    while (curr != nullptr) {
        file << curr->data << " ";
        curr = curr->next;
    }
}

void Queue::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string value;
    while (file >> value) {
        enqueue(value);
    }
}

void Queue::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    QueueNode* curr = front;
    while (curr != nullptr) {
        size_t strSize = curr->data.size();
        auto streamSize = static_cast<streamsize>(strSize);
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(curr->data.c_str(), streamSize);
        curr = curr->next;
    }
}

void Queue::loadFromBinaryFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    while (file.peek() != EOF) {
        size_t strSize;
        file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
        if (file.eof()){ 
            break;
        }
        auto streamSize = static_cast<streamsize>(strSize);
        char* buffer = new char[strSize + 1];
        file.read(buffer, streamSize);
        buffer[strSize] = '\0';
        enqueue(string(buffer));
        delete[] buffer;
    }
}