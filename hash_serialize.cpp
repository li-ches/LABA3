#include "hash_serialize.h"
#include "serialize.h"
#include <iostream>

using namespace std;

// Сохранение ChainHash в текстовый файл
void HashSerializer::saveToFile(const ChainHash& table, const string& filename) {
    ofstream file(filename);
    if (!file) return;

    file << table.cap << "\n";

    int count = 0;
    for (int i = 0; i < table.cap; ++i) {
        ChainHash::Node* curr = table.table[i];
        while (curr) {
            count++;
            curr = curr->next;
        }
    }
    file << count << "\n";
    
    for (int i = 0; i < table.cap; ++i) {
        ChainHash::Node* curr = table.table[i];
        while (curr) {
            writeStringText(file, curr->key);
            writeStringText(file, curr->val);
            curr = curr->next;
        }
    }
}

// Загрузка ChainHash из текстового файла
void HashSerializer::loadFromFile(ChainHash& table, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    int newCap;
    file >> newCap;
    string dummy; getline(file, dummy);
    
    table.clearAndResize(newCap);
    
    int count;
    file >> count;
    getline(file, dummy);
    
    for (int i = 0; i < count; ++i) {
        string key = readStringText(file);
        string val = readStringText(file);
        if (file.fail()) break;
        table.insert(key, val);
    }
}

// Бинарное сохранение ChainHash
void HashSerializer::saveToBinaryFile(const ChainHash& table, const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(&table.cap), sizeof(table.cap));
    
    int count = 0;
    for (int i = 0; i < table.cap; ++i) {
        ChainHash::Node* curr = table.table[i];
        while (curr) {
            count++;
            curr = curr->next;
        }
    }
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    for (int i = 0; i < table.cap; ++i) {
        ChainHash::Node* curr = table.table[i];
        while (curr) {
            writeString(file, curr->key);
            writeString(file, curr->val);
            curr = curr->next;
        }
    }
}

// Бинарная загрузка ChainHash
void HashSerializer::loadFromBinaryFile(ChainHash& table, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;
    
    int newCap;
    file.read(reinterpret_cast<char*>(&newCap), sizeof(newCap));
    table.clearAndResize(newCap);
    
    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    if (file.fail()) return;
    for (int i = 0; i < count; ++i) {
        string key = readString(file);
        string val = readString(file);
        if (file.fail()) break;
        table.insert(key, val);
    }
}

// Сохранение OpenHash в текстовый файл
void HashSerializer::saveToFile(const OpenHash& table, const string& filename) {
    ofstream file(filename);
    if (!file) return;
    file << table.cap << "\n";
    
    int count = 0;
    for (int i = 0; i < table.cap; ++i) {
        if (table.table[i].used) count++;
    }
    file << count << "\n";

    for (int i = 0; i < table.cap; ++i) {
        if (table.table[i].used) {
            writeStringText(file, table.table[i].key);
            writeStringText(file, table.table[i].val);
        }
    }
}

// Загрузка OpenHash из текстового файла
void HashSerializer::loadFromFile(OpenHash& table, const string& filename) {
    ifstream file(filename);
    if (!file) return;
    
    int newCap;
    file >> newCap;
    string dummy; getline(file, dummy);
    
    table.clearAndResize(newCap);
    
    int count;
    file >> count;
    getline(file, dummy);
    
    if (file.fail()) return;
    for (int i = 0; i < count; ++i) {
        string key = readStringText(file);
        string val = readStringText(file);
        if (file.fail()) break;
        table.insert(key, val);
    }
}

// Бинарное сохранение OpenHash
void HashSerializer::saveToBinaryFile(const OpenHash& table, const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(&table.cap), sizeof(table.cap));
    
    int count = 0;
    for (int i = 0; i < table.cap; ++i) {
        if (table.table[i].used) count++;
    }
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    for (int i = 0; i < table.cap; ++i) {
        if (table.table[i].used) {
            writeString(file, table.table[i].key);
            writeString(file, table.table[i].val);
        }
    }
}

// Бинарная загрузка OpenHash
void HashSerializer::loadFromBinaryFile(OpenHash& table, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;
    
    int newCap;
    file.read(reinterpret_cast<char*>(&newCap), sizeof(newCap));
    table.clearAndResize(newCap);
    
    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    for (int i = 0; i < count; ++i) {
        string key = readString(file);
        string val = readString(file);
        if (file.fail()) break;
        table.insert(key, val);
    }
}