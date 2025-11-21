#include <iostream>
#include <fstream>
#include <vector>
#include "node.h"
#include "stack.h"

using namespace std;

Stack::Stack() {
    top = nullptr;
}

Stack::~Stack() {
    while (top != nullptr) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

void Stack::push(const string& val) {
    auto* newNode = new StackNode(val);
    newNode->next = top;
    top = newNode;
}

void Stack::pop() {
    if (top == nullptr) {
        throw runtime_error("error");
    }
    StackNode* temp = top;
    top = top->next;
    delete temp;
}

void Stack::print() const {
    if (top == nullptr) {
        cout << "error" << '\n';
        return;
    }
    
    StackNode* curr = top;
    cout << "стек: ";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << '\n';
}

void Stack::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    StackNode* curr = top;
    while (curr != nullptr) {
        file << curr->data << " ";
        curr = curr->next;
    }
}

void Stack::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    vector<string> elements;
    string value;
    while (file >> value) {
        elements.push_back(value);
    }

    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        push(*it);
    }
}

void Stack::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    StackNode* curr = top;
    while (curr != nullptr) {
        size_t strSize = curr->data.size();
        auto streamSize = static_cast<streamsize>(strSize);
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(curr->data.c_str(), streamSize);
        curr = curr->next;
    }
}

void Stack::loadFromBinaryFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    vector<string> elements;
    
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
        
        elements.emplace_back(buffer);
        delete[] buffer;
    }
    
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        push(*it);
    }
}