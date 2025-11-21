#include <iostream>
#include <fstream>
#include "dynarr.h"

using namespace std;

DynArr::DynArr(int initialcap) {
    data = new string[initialcap];
    cap = initialcap;
    len = 0;
}

DynArr::~DynArr() {
    delete[] data;
}

void DynArr::addToEnd(const string& val) {
    if (len >= cap) {
        int newcap = cap * 2;
        auto* newData = new string[newcap];
        for (int i = 0; i < len; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newcap;
    }
    data[len] = val;
    len++;
}

void DynArr::addAtIndex(int index, const string& val) {
    if (index < 0 || index > len) {
        throw runtime_error("error");
    }
    
    if (len >= cap) {
        int newcap = cap * 2;
        auto* newData = new string[newcap];
        for (int i = 0; i < len; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newcap;
    }
    
    for (int i = len; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = val;
    len++;
}

auto DynArr::getElement(int index) const -> string {
    if (index < 0 || index >= len) {
        throw runtime_error("error");
    }
    return data[index];
}

void DynArr::replaceElement(int index, const string& value) const {
    if (index < 0 || index >= len) {
        throw runtime_error("error");
    }
    data[index] = value;
}

void DynArr::removeAtIndex(int index) {
    if (index < 0 || index >= len) {
        throw runtime_error("error");
    }
    
    for (int i = index; i < len - 1; i++) {
        data[i] = data[i + 1];
    }
    len--;
}

void DynArr::printArr() const {
    if (len == 0) {
        cout << "error" << '\n';
        return;
    }
    
    cout << "[";
    for (int i = 0; i < len; i++) {
        cout << data[i];
        if (i < len - 1) {
            cout << ", ";
        }
    }
    cout << "]" << '\n';
}

void DynArr::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < len; i++) {
        file << data[i] << " ";
    }
}

void DynArr::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string value;
    while (file >> value) {
        addToEnd(value);
    }
}

void DynArr::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < len; i++) {
        size_t strSize = data[i].size();
        auto streamSize = static_cast<streamsize>(strSize);
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(data[i].c_str(), streamSize);
    }
}

void DynArr::loadFromBinaryFile(ifstream& file) {
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
        addToEnd(string(buffer));
        delete[] buffer;
    }
}