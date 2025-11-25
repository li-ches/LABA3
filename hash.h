#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

auto hash_str(const string& key, int mod) -> int;

class ChainHash {
private:
    class Node {
    public:
        string key;
        string val;
        Node* next;

        Node(const string& keyArg, const string& valArg)
            : key(keyArg), val(valArg), next(nullptr) {}
    };

    Node** table;
    int cap;

public:
    explicit ChainHash(int buckCount);
    ~ChainHash();

    ChainHash(const ChainHash&) = delete;
    ChainHash& operator=(const ChainHash&) = delete;
    ChainHash(ChainHash&&) = delete;
    ChainHash& operator=(ChainHash&&) = delete;

    void insert(const string& key, const string& val);

    auto erase(const string& key) -> bool;
    auto find(const string& key) -> string;

    void show();

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);

    void saveToBinaryFile(const string& filename) const;
    void loadFromBinaryFile(const string& filename);
};

class OpenHash {
private:
    class Entry {
    public:
        string key;
        string val;
        bool used;
        bool deleted;

        Entry() : used(false), deleted(false) {}
    };

    Entry* table;
    int cap;

public:
    explicit OpenHash(int size);
    ~OpenHash();

    void insert(const string& key, const string& value);

    auto erase(const string& key) -> bool;
    auto find(const string& key) const -> string;

    void show();

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);

    void saveToBinaryFile(const string& filename) const;
    void loadFromBinaryFile(const string& filename);
};

void hash_man();
void parse_cmd(const std::string& line, std::string& cmd, std::string& arg1, std::string& arg2);

#endif
