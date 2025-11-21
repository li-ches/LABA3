#include <iostream>
#include <fstream>
#include "node.h"
#include "onelist.h"

using namespace std;

OneList::OneList() {
    head = nullptr;
    tail = nullptr;
}

OneList::~OneList() {
    OneListNode* curr = head;
    while (curr != nullptr) {
        OneListNode* next = curr->next;
        delete curr;
        curr = next;
    }
}

void OneList::addToHead(const string& val) {
    auto* newNode = new OneListNode(val);
    newNode->next = head;   
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
}

void OneList::addToTail(const string& val) {
    auto *newNode = new OneListNode(val);
    newNode->next = nullptr;    
    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode; 
    }
}

void OneList::addAfterValue(const string& targetVal, const string& newVal) {
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }

    auto *newNode = new OneListNode(newVal);
    newNode->next = targetNode->next;
    targetNode->next = newNode;
    
    if (targetNode == tail) {
        tail = newNode;
    }
}

void OneList::addBeforeValue(const string& targetVal, const string& newVal) {
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    
    if (targetNode == head) {
        addToHead(newVal);
        return;
    }
    
    OneListNode* prevNode = head;
    while (prevNode != nullptr && prevNode->next != targetNode) {
        prevNode = prevNode->next;
    }

    if (prevNode == nullptr) {
        throw runtime_error("error");
    }

    auto *newNode = new OneListNode(newVal);
    newNode->next = targetNode;
    prevNode->next = newNode;
}

void OneList::removeFromHead() {
    if (head == nullptr) {
        throw runtime_error("error");
    }  
    OneListNode* temp = head;
    head = head->next;   
    if (head == nullptr) {
        tail = nullptr;
    }    
    delete temp;
}

void OneList::removeFromTail() {
    if (head == nullptr) {
        throw runtime_error("error");
    }  
    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
    } else {
        OneListNode* curr = head;
        while (curr->next != tail) {
            curr = curr->next;
        }     
        delete tail;
        tail = curr; 
        tail->next = nullptr;
    }
}

void OneList::removeAfterValue(const string& targetVal) {
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr || targetNode->next == nullptr) {
        throw runtime_error("error");
    }
    
    OneListNode* temp = targetNode->next;
    targetNode->next = temp->next;
    
    if (temp == tail) {
        tail = targetNode;
    }
    
    delete temp;
}

void OneList::removeBeforeValue(const string& targetVal) {
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr || targetNode == head) {
        throw runtime_error("error");
    }
    
    if (head->next == targetNode) {
        removeFromHead();
        return;
    }
    OneListNode* prevPrev = head;
    while (prevPrev->next != nullptr && prevPrev->next->next != targetNode) {
        prevPrev = prevPrev->next;
    }
    
    OneListNode* temp = prevPrev->next;
    prevPrev->next = targetNode;
    
    delete temp;
}

void OneList::removeByValue(const string& targetVal) {
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    
    if (targetNode == head) {
        removeFromHead();
        return;
    }

    OneListNode* prevNode = head;
    while (prevNode != nullptr && prevNode->next != targetNode) {
        prevNode = prevNode->next;
    }

    if (prevNode == nullptr) {
        throw runtime_error("error");
    }

    prevNode->next = targetNode->next;
    
    if (targetNode == tail) {
        tail = prevNode;
    }
    
    delete targetNode;
}

auto OneList::getElementByValue(const string &targetVal) const -> string
{
    OneListNode* targetNode = findByVal(targetVal);
    if (targetNode == nullptr) {
        throw runtime_error("error");
    }
    return targetNode->data;
}

auto OneList::findByVal(const string &val) const -> OneListNode *
{
    OneListNode* curr = head;   
    while (curr != nullptr) {
        if (curr->data == val) {
            return curr;  
        }
        curr = curr->next;
    }    
    return nullptr;
}

void OneList::print() const {
    if (head == nullptr) {
        cout << "error" << '\n';
        return;
    }
    
    OneListNode* curr = head;
    cout << "список: ";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << '\n';
}

void OneList::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    OneListNode* curr = head;
    while (curr != nullptr) {
        file << curr->data << " ";
        curr = curr->next;
    }
}

void OneList::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string value;
    while (file >> value) {
        addToTail(value);
    }
}

void OneList::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    OneListNode* curr = head;
    while (curr != nullptr) {
        size_t strSize = curr->data.size();
        auto streamSize = static_cast<streamsize>(strSize);
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(curr->data.c_str(), streamSize);
        curr = curr->next;
    }
}

void OneList::loadFromBinaryFile(ifstream& file) {
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