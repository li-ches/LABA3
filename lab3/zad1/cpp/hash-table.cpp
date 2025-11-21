#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "node.h"
#include "hash-table.h"

using namespace std;

// ==================== КУКУШКА ================

auto CuckooHashTable::hash1(const string& key) const -> int{
    int hash = 0;
    for (char cha : key) {
        hash = (hash * Config::FIRST_HASH + cha) % capacity;
    }
    return (hash % capacity + capacity) % capacity;
}

auto CuckooHashTable::hash2(const string& key) const -> int{
    int hash = 0;
    for (char cha : key) {
        hash = (hash * Config::SECOND_HASH + cha) % capacity;
    }
    return (hash % capacity + capacity) % capacity;
}

CuckooHashTable::CuckooHashTable(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->maxIterations = capacity * 2;
    
    table1 = new CKNode[capacity];
    table2 = new CKNode[capacity];
    
    for (int i = 0; i < capacity; i++) {
        table1[i].isEmpty = true;
        table1[i].isDeleted = false;
        table1[i].key = "";    
        table1[i].data = "";
        table2[i].isEmpty = true;
        table2[i].isDeleted = false;
        table2[i].key = "";    
        table2[i].data = "";
    }
}

CuckooHashTable::~CuckooHashTable() {
    delete[] table1;
    delete[] table2;
}

auto CuckooHashTable::rehash(int newCapacity) -> bool {
    vector<pair<string, string>> elements;
    for (int i = 0; i < capacity; i++) {
        if (!table1[i].isEmpty && !table1[i].isDeleted) {
            elements.emplace_back(table1[i].key, table1[i].data);
        }
        if (!table2[i].isEmpty && !table2[i].isDeleted) {
            elements.emplace_back(table2[i].key, table2[i].data);
        }
    }
    
    CKNode* oldTable1 = table1;
    CKNode* oldTable2 = table2;
    table1 = new CKNode[newCapacity];
    table2 = new CKNode[newCapacity];
    capacity = newCapacity;
    size = 0;
    maxIterations = newCapacity * 2;
    
    for (int i = 0; i < newCapacity; i++) {
        table1[i].isEmpty = true;
        table1[i].isDeleted = false;
        table2[i].isEmpty = true;
        table2[i].isDeleted = false;
    }

    bool success = true;
    for (const auto& elem : elements) {
        string key = elem.first;
        string value = elem.second;
        if (!insert(key, value)) {
            success = false;
            break;
        }
    }

    delete[] oldTable1;
    delete[] oldTable2;
    
    return success;
}

auto CuckooHashTable::insert(const string& key, string& data) -> bool {
    if (key.empty()) {
        return false;
    }
    
    int pos1 = hash1(key);
    int pos2 = hash2(key);
    
    if (!table1[pos1].isEmpty && !table1[pos1].isDeleted && table1[pos1].key == key) {
        table1[pos1].data = data;
        return true;
    }
    if (!table2[pos2].isEmpty && !table2[pos2].isDeleted && table2[pos2].key == key) {
        table2[pos2].data = data; 
        return true;
    }

    double loadFactor = (double)size / capacity;
    if (loadFactor > Config::LOAD_FACTOR_COO) {
        if (!rehash(capacity * 2)) {
            return false;
        }
    }

    string currentKey = key;
    string currentData = data;
    int iterations = 0;

    while (iterations < maxIterations) {
        int pos1 = hash1(currentKey);
        if (table1[pos1].isEmpty || table1[pos1].isDeleted) {
            table1[pos1].key = currentKey;
            table1[pos1].data = currentData;
            table1[pos1].isEmpty = false;
            table1[pos1].isDeleted = false;
            size++;
            return true;
        }

        swap(currentKey, table1[pos1].key);
        swap(currentData, table1[pos1].data);

        int pos2 = hash2(currentKey);
        if (table2[pos2].isEmpty || table2[pos2].isDeleted) {
            table2[pos2].key = currentKey;
            table2[pos2].data = currentData;
            table2[pos2].isEmpty = false;
            table2[pos2].isDeleted = false;
            size++;
            return true;
        }

        swap(currentKey, table2[pos2].key);
        swap(currentData, table2[pos2].data);
        
        iterations++;
    }

    if (iterations >= maxIterations) {
        if (rehash(capacity * 2)) {
            return insert(key, data);
        }
    }
    
    return false;
}

auto CuckooHashTable::search(const string& key) const -> string {
    if (key.empty()) {
        return ""; 
    }
    int pos1 = hash1(key);
    int pos2 = hash2(key);
    
    if (!table1[pos1].isEmpty && !table1[pos1].isDeleted && table1[pos1].key == key) {
        return table1[pos1].data;
    }
    if (!table2[pos2].isEmpty && !table2[pos2].isDeleted && table2[pos2].key == key) {
        return table2[pos2].data; 
    }
    return ""; 
}

auto CuckooHashTable::remove(const string& key) -> bool {
    if (key.empty()) {
        return false;
    }
    int pos1 = hash1(key);
    int pos2 = hash2(key);
    
    if (!table1[pos1].isEmpty && !table1[pos1].isDeleted && table1[pos1].key == key) {
        table1[pos1].isDeleted = true;
        size--;
        return true;
    }
    if (!table2[pos2].isEmpty && !table2[pos2].isDeleted && table2[pos2].key == key) {
        table2[pos2].isDeleted = true;
        size--;
        return true;
    }
    return false;
}

void CuckooHashTable::print() const {
    cout << "=== Кукушкина таблица ===" << '\n';
    cout << "Размер: " << size << "/" << capacity << " (нагрузка: " 
         << (double)size / capacity * Config::PROC << "%)" << '\n';
    
    cout << "Таблица 1: ";
    for (int i = 0; i < capacity; i++) {
        if (!table1[i].isEmpty && !table1[i].isDeleted) {
            cout << "[" << i << "]:'" << table1[i].key << " -> " << table1[i].data << "' ";
        }
    }
    cout << '\n';
    cout << "Таблица 2: ";
    for (int i = 0; i < capacity; i++) {
        if (!table2[i].isEmpty && !table2[i].isDeleted) {
            cout << "[" << i << "]:'" << table2[i].key << " -> " << table2[i].data << "' ";
        }
    }
    cout << '\n';
}

auto CuckooHashTable::getAllElements() const -> vector<pair<string, string>>  
{
    vector<pair<string, string>> elements;
    for (int i = 0; i < capacity; i++) {
        if (!table1[i].isEmpty && !table1[i].isDeleted) {
            elements.emplace_back(table1[i].key, table1[i].data);
        }
        if (!table2[i].isEmpty && !table2[i].isDeleted) {
            elements.emplace_back(table2[i].key, table2[i].data);
        }
    }
    return elements;
}

void CuckooHashTable::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < capacity; i++) {
        if (!table1[i].isEmpty && !table1[i].isDeleted) {
            file << table1[i].key << ":" << table1[i].data << " ";
        }
        if (!table2[i].isEmpty && !table2[i].isDeleted) {
            file << table2[i].key << ":" << table2[i].data << " ";
        }
    }
}

void CuckooHashTable::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string keyValue;
    while (file >> keyValue) {
        size_t colonPos = keyValue.find(':');
        if (colonPos != string::npos) {
            string key = keyValue.substr(0, colonPos);
            string value = keyValue.substr(colonPos + 1);
            static_cast<void>(insert(key, value));
        }
    }
}

void CuckooHashTable::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < capacity; i++) {
        if (!table1[i].isEmpty && !table1[i].isDeleted) {
            size_t keySize = table1[i].key.size();
            auto streamSize = static_cast<streamsize>(keySize);
            file.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            file.write(table1[i].key.c_str(), streamSize);
            
            size_t dataSize = table1[i].data.size();
            auto dataStreamSize = static_cast<streamsize>(dataSize);
            file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
            file.write(table1[i].data.c_str(), dataStreamSize);
        }
        if (!table2[i].isEmpty && !table2[i].isDeleted) {
            size_t keySize = table2[i].key.size();
            auto streamSize = static_cast<streamsize>(keySize);
            file.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            file.write(table2[i].key.c_str(), streamSize);
            
            size_t dataSize = table2[i].data.size();
            auto dataStreamSize = static_cast<streamsize>(dataSize);
            file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
            file.write(table2[i].data.c_str(), dataStreamSize);
        }
    }
}

void CuckooHashTable::loadFromBinaryFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    while (file.peek() != EOF) {
        size_t keySize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        if (file.eof()){ 
            break;
        }
        auto streamSize = static_cast<streamsize>(keySize);
        char* keyBuffer = new char[keySize + 1];
        file.read(keyBuffer, streamSize);
        keyBuffer[keySize] = '\0';
        string key(keyBuffer);
        delete[] keyBuffer;
        
        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        auto dataStreamSize = static_cast<streamsize>(dataSize);
        char* dataBuffer = new char[dataSize + 1];
        file.read(dataBuffer, dataStreamSize);
        dataBuffer[dataSize] = '\0';
        string value(dataBuffer);
        delete[] dataBuffer;
        
        static_cast<void>(insert(key, value));
    }
}

// ==================== ДВОЙНОЕ ХЕШИРОВАНИЕ ====================

auto DoubleHashTable::hash1(const string& key) const -> int{
    int hash = 0;
    for (char cha : key) {
        hash = (hash * Config::FIRST_HASH + cha) % capacity;
    }
    return (hash % capacity + capacity) % capacity;
}

auto DoubleHashTable::hash2(const string& key) const -> int{
    int hash = 0;
    for (char cha : key) {
        hash = (hash * Config::SECOND_HASH + cha) % capacity;
    }
    return (hash % capacity + capacity) % capacity;
}

DoubleHashTable::DoubleHashTable(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    
    table = new DHNode[capacity];
    
    for (int i = 0; i < capacity; i++) {
        table[i].isEmpty = true;
        table[i].isDeleted = false;
        table[i].key = "";    
        table[i].data = "";
    }
}

DoubleHashTable::~DoubleHashTable() {
    delete[] table;
}

auto DoubleHashTable::rehash(int newCapacity) -> bool {
    vector<pair<string, string>> elements;
    for (int i = 0; i < capacity; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            elements.emplace_back(table[i].key, table[i].data);
        }
    }

    DHNode* oldTable = table;
    table = new DHNode[newCapacity];
    capacity = newCapacity;
    size = 0;

    for (int i = 0; i < newCapacity; i++) {
        table[i].isEmpty = true;
        table[i].isDeleted = false;
    }
    
    bool success = true;
    for (const auto& elem : elements) {
        string key = elem.first;
        string value = elem.second;
        if (!insert(key, value)) {
            success = false;
            break;
        }
    }

    delete[] oldTable;
    
    return success;
}

auto DoubleHashTable::insert(const string& key, string& data) -> bool {
    if (key.empty()) {
        return false;
    }
    
    double loadFactor = (double)size / capacity;
    if (loadFactor > Config::LOAD_FACTOR_DOUBLE) {
        if (!rehash(capacity * 2)) {
            return false;
        }
    }
    
    int hazh1 = hash1(key);
    int hazh2 = hash2(key);
    int index = hazh1;
    
    for (int i = 0; i < capacity; i++) {
        if (table[index].isEmpty || table[index].isDeleted) {
            table[index].key = key;
            table[index].data = data;
            table[index].isEmpty = false;
            table[index].isDeleted = false;
            size++;
            return true;
        }      
        if (!table[index].isEmpty && !table[index].isDeleted && table[index].key == key) {
            table[index].data = data; 
            return true;
        }

        index = (hazh1 + i * hazh2) % capacity;
    }

    if (rehash(capacity * 2)) {
        return insert(key, data);
    }
    
    return false;
}

auto DoubleHashTable::search(const string& key) const -> string {
    if (key.empty()) {
        return ""; 
    }
    int hazh1 = hash1(key);
    int hazh2 = hash2(key);
    int index = hazh1;
    for (int i = 0; i < capacity; i++) {
        if (table[index].isEmpty && !table[index].isDeleted) {
            break;
        }      
        if (!table[index].isEmpty && !table[index].isDeleted && table[index].key == key) {
            return table[index].data; 
        }
        index = (hazh1 + i * hazh2) % capacity;
    }
    return ""; 
}

auto DoubleHashTable::remove(const string& key) -> bool {
    if (key.empty()) {
        return false;
    }
    int hazh1 = hash1(key);
    int hazh2 = hash2(key);
    int index = hazh1;

    for (int i = 0; i < capacity; i++) {
        if (table[index].isEmpty && !table[index].isDeleted) {
            break;
        }
        
        if (!table[index].isEmpty && !table[index].isDeleted && table[index].key == key) {
            table[index].isDeleted = true;
            size--;
            return true;
        }

        index = (hazh1 + i * hazh2) % capacity;
    }
    return false;
}

void DoubleHashTable::print() const {
    cout << "=== Таблица двойного хеширования ===" << '\n';
    cout << "Размер: " << size << "/" << capacity << " (нагрузка: " 
         << (double)size / capacity * Config::PROC << "%)" << '\n';
    
    for (int i = 0; i < capacity; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            cout << "[" << i << "]:'" << table[i].key << " -> " << table[i].data << "' ";
        }
    }
    cout << '\n';
}

auto DoubleHashTable::getAllElements() const -> vector<pair<string, string>> {
    vector<pair<string, string>> elements;
    for (int i = 0; i < capacity; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            elements.emplace_back(table[i].key, table[i].data);
        }
    }
    return elements;
}

void DoubleHashTable::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < capacity; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            file << table[i].key << ":" << table[i].data << " ";
        }
    }
}

void DoubleHashTable::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    string keyValue;
    while (file >> keyValue) {
        size_t colonPos = keyValue.find(':');
        if (colonPos != string::npos) {
            string key = keyValue.substr(0, colonPos);
            string value = keyValue.substr(colonPos + 1);
            static_cast<void>(insert(key, value));
        }
    }
}

void DoubleHashTable::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < capacity; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            size_t keySize = table[i].key.size();
            auto streamSize = static_cast<streamsize>(keySize);
            file.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            file.write(table[i].key.c_str(), streamSize);
            
            size_t dataSize = table[i].data.size();
            auto dataStreamSize = static_cast<streamsize>(dataSize);
            file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
            file.write(table[i].data.c_str(), dataStreamSize);
        }
    }
}

void DoubleHashTable::loadFromBinaryFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    while (file.peek() != EOF) {
        size_t keySize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        if (file.eof()){ 
            break;
        }
        auto streamSize = static_cast<streamsize>(keySize);
        char* keyBuffer = new char[keySize + 1];
        file.read(keyBuffer, streamSize);
        keyBuffer[keySize] = '\0';
        string key(keyBuffer);
        delete[] keyBuffer;
        
        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        auto dataStreamSize = static_cast<streamsize>(dataSize);
        char* dataBuffer = new char[dataSize + 1];
        file.read(dataBuffer, dataStreamSize);
        dataBuffer[dataSize] = '\0';
        string value(dataBuffer);
        delete[] dataBuffer;
        
        static_cast<void>(insert(key, value));
    }
}
