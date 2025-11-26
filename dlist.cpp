#include "dlist.h"
#include <iostream>
#include <fstream>

using namespace std;

DList::DList() : head(nullptr), tail(nullptr) {}

DList::~DList() {
    while (head) delHead();
}

void DList::addHead(const string& val) {
    DNode* newNode = new DNode(val, head, nullptr);
    if (head) head->prev = newNode;
    head = newNode;
    if (!tail) tail = newNode;
}

void DList::addTail(const string& val) {
    DNode* newNode = new DNode(val, nullptr, tail);
    if (tail) tail->next = newNode;
    tail = newNode;
    if (!head) head = newNode;
}

auto DList::findValue(const string& val) const -> DNode* {
    DNode* curr = head;
    while (curr) {
        if (curr->value == val) return curr;
        curr = curr->next;
    }
    return nullptr;
}

void DList::addBefore(const string& target, const string& val) {
    DNode* targetNode = findValue(target);
    if (!targetNode) {
        cout << "Элемент '" << target << "' не найден." << endl;
        return;
    }
    DNode* newNode = new DNode(val, targetNode, targetNode->prev);
    if (targetNode->prev) targetNode->prev->next = newNode;
    else head = newNode;
    targetNode->prev = newNode;
}

void DList::addAfter(const string& target, const string& val) {
    DNode* targetNode = findValue(target);
    if (!targetNode) {
        cout << "Элемент '" << target << "' не найден." << endl;
        return;
    }
    DNode* newNode = new DNode(val, targetNode->next, targetNode);
    if (targetNode->next) targetNode->next->prev = newNode;
    else tail = newNode;
    targetNode->next = newNode;
}

void DList::delHead() {
    if (!head) {
        cout << "Список пуст. Удаление невозможно." << endl;
        return;
    }
    DNode* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete temp;
}

void DList::delTail() {
    if (!tail) {
        cout << "Список пуст. Удаление невозможно." << endl;
        return;
    }
    DNode* temp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete temp;
}

void DList::delByVal(const string& val) {
    DNode* targetNode = findValue(val);
    if (!targetNode) {
        cout << "Элемент '" << val << "' не найден." << endl;
        return;
    }
    if (targetNode->prev) targetNode->prev->next = targetNode->next;
    else head = targetNode->next;
    if (targetNode->next) targetNode->next->prev = targetNode->prev;
    else tail = targetNode->prev;
    delete targetNode;
}

auto DList::contains(const string& val) const -> bool {
    return findValue(val) != nullptr;
}

void DList::readForward() const {
    if (!head) {
        cout << "Список пуст." << endl;
        return;
    }
    cout << "Список: ";
    DNode* curr = head;
    while (curr) {
        cout << curr->value;
        if (curr->next) cout << " <-> ";
        curr = curr->next;
    }
    cout << endl;
}

void DList::readBackward() const {
    if (!tail) {
        cout << "Список пуст." << endl;
        return;
    }
    cout << "Список (обратный): ";
    DNode* curr = tail;
    while (curr) {
        cout << curr->value;
        if (curr->prev) cout << " <-> ";
        curr = curr->prev;
    }
    cout << endl;
}

void DList::delAfterValue(const string& val) {
    DNode* targetNode = findValue(val);
    if (!targetNode || !targetNode->next) {
        cout << "Невозможно удалить элемент после '" << val << "'." << endl;
        return;
    }
    DNode* toDelete = targetNode->next;
    targetNode->next = toDelete->next;
    if (toDelete->next) toDelete->next->prev = targetNode;
    else tail = targetNode;
    delete toDelete;
}

void DList::delBeforeValue(const string& val) {
    DNode* targetNode = findValue(val);
    if (!targetNode || !targetNode->prev) {
        cout << "Невозможно удалить элемент перед '" << val << "'." << endl;
        return;
    }
    DNode* toDelete = targetNode->prev;
    targetNode->prev = toDelete->prev;
    if (toDelete->prev) toDelete->prev->next = targetNode;
    else head = targetNode;
    delete toDelete;
}
