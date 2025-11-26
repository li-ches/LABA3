#include "arr.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

void MyArr::ensureCapacity(int newSize) {
    if (newSize <= capacity) return;
    
    int newCapacity = (capacity == 0) ? 4 : capacity * 2;
    while (newCapacity < newSize) newCapacity *= 2;
    
    string* newData = new string[newCapacity];
    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

MyArr::MyArr() : data(nullptr), size(0), capacity(0) {}

MyArr::~MyArr() {
    delete[] data;
}

void MyArr::addHead(const string& val) {
    ensureCapacity(size + 1);
    for (int i = size; i > 0; --i) {
        data[i] = data[i - 1];
    }
    data[0] = val;
    ++size;
}

void MyArr::delHead() {
    if (size == 0) return;
    for (int i = 0; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

void MyArr::delEnd() {
    if (size == 0) return;
    --size;
}

void MyArr::addEnd(const string& val) {
    ensureCapacity(size + 1);
    data[size] = val;
    ++size;
}

string MyArr::getAt(int idx) const {
    if (idx < 0 || idx >= size) return "[INVALID_INDEX]";
    return data[idx];
}

void MyArr::delAt(int idx) {
    if (idx < 0 || idx >= size) return;
    for (int i = idx; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

void MyArr::readArray() const {
    if (size == 0) {
        cout << "Массив пуст." << endl;
        return;
    }
    cout << "Массив [" << size << "]: ";
    for (int i = 0; i < size; ++i) {
        cout << data[i];
        if (i < size - 1) cout << " -> ";
    }
    cout << endl;
}

void MyArr::addAt(int idx, const string& val) {
    if (idx < 0 || idx > size) return;
    ensureCapacity(size + 1);
    for (int i = size; i > idx; --i) {
        data[i] = data[i - 1];
    }
    data[idx] = val;
    ++size;
}

void MyArr::repArr(int idx, const string& val) {
    if (idx < 0 || idx >= size) return;
    data[idx] = val;
}

auto MyArr::lenArr() const -> int {
    return size;
}
