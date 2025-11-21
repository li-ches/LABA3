#include <iostream>
#include <fstream>
#include "node.h"
#include "twolist.h"

using namespace std;

TwoList::TwoList() {
    head = nullptr;
    tail = nullptr;
}

TwoList::~TwoList() {
    TwoListNode* curr = head;
    while (curr != nullptr) {
        TwoListNode* next = curr->next;
        delete curr;
        curr = next;
    }
}

void TwoList::addToHead(const string& val) {
    auto *newNode = new TwoListNode(val);
    newNode->prev = nullptr;    
    if (head == nullptr) {
        newNode->next = nullptr;
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

void TwoList::addToTail(const string& val) {
    auto *newNode = new TwoListNode(val);
    newNode->next = nullptr;
    if (head == nullptr) {
        newNode->prev = nullptr;
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

void TwoList::addAfterValue(const string& targetVal, const string& newVal) {
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }

    auto *newNode = new TwoListNode(newVal);
    newNode->next = targetNode->next;
    newNode->prev = targetNode;
    
    if (targetNode->next != nullptr) {
        targetNode->next->prev = newNode;
    }
    targetNode->next = newNode;
    
    if (targetNode == tail) {
        tail = newNode;
    }
}

void TwoList::addBeforeValue(const string& targetVal, const string& newVal) {
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    
    if (targetNode == head) {
        addToHead(newVal);
        return;
    }

    auto *newNode = new TwoListNode(newVal);
    newNode->next = targetNode;
    newNode->prev = targetNode->prev;
    
    if (targetNode->prev != nullptr) {
        targetNode->prev->next = newNode;
    }
    targetNode->prev = newNode;
}

void TwoList::removeFromHead() {
    if (head == nullptr) {
        throw runtime_error("error");
    }    
    TwoListNode* temp = head;    
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }    
    delete temp;
}

void TwoList::removeFromTail() {
    if (head == nullptr) {
        throw runtime_error("error");
    }
    TwoListNode* temp = tail;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete temp;
}

void TwoList::removeAfterValue(const string& targetVal) {
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr || targetNode->next == nullptr) {
        throw runtime_error("error");
    }
    
    TwoListNode* temp = targetNode->next;
    targetNode->next = temp->next;
    
    if (temp->next != nullptr) {
        temp->next->prev = targetNode;
    }
    
    if (temp == tail) {
        tail = targetNode;
    }
    
    delete temp;
}

void TwoList::removeBeforeValue(const string& targetVal) {
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr || targetNode->prev == nullptr) {
        throw runtime_error("error");
    }
    
    if (targetNode->prev == head) {
        removeFromHead();
        return;
    }
    
    TwoListNode* temp = targetNode->prev;
    targetNode->prev = temp->prev;
    
    if (temp->prev != nullptr) {
        temp->prev->next = targetNode;
    }
    
    delete temp;
}

void TwoList::removeByValue(const string& targetVal) {
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    
    if (targetNode == head) {
        removeFromHead();
        return;
    }
    
    if (targetNode == tail) {
        removeFromTail();
        return;
    }
    
    if (targetNode->prev != nullptr) {
        targetNode->prev->next = targetNode->next;
    }
    
    if (targetNode->next != nullptr) {
        targetNode->next->prev = targetNode->prev;
    }
    
    delete targetNode;
}

auto TwoList::getElementByValue(const string &targetVal) const -> string 
{
    TwoListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    return targetNode->data;
}

auto TwoList::findByVal(const string &val) const -> TwoListNode *
{
    TwoListNode* curr = head;    
    while (curr != nullptr) {
        if (curr->data == val) {
            return curr;
        }
        curr = curr->next;
    }   
    return nullptr;
}

void TwoList::print() const {
    if (head == nullptr) {
        cout << "error" << '\n';
        return;
    }    
    TwoListNode* curr = head;
    cout << "список: ";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " <-> ";
        }
        curr = curr->next;
    }
    cout << '\n';
}


void TwoList::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    TwoListNode* current = head;
    while (current != nullptr) {
        file << current->data << " ";
        current = current->next;
    }
}

void TwoList::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string value;
    while (file >> value) {
        addToTail(value);
    }
}

void TwoList::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    TwoListNode* current = head;
    while (current != nullptr) {
        size_t strSize = current->data.size();
        auto streamSize = static_cast<streamsize>(strSize);
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(current->data.c_str(), streamSize);
        current = current->next;
    }
}

void TwoList::loadFromBinaryFile(ifstream& file) {
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
        addToTail(string(buffer));
        delete[] buffer;
    }
}