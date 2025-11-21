#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <fstream>

namespace Config {
    constexpr int DEFAULT_INITIAL_CAP_HASH = 10;
    constexpr int FIRST_HASH = 59;
    constexpr int SECOND_HASH = 41;
    constexpr double LOAD_FACTOR_COO = 0.5;
    constexpr double LOAD_FACTOR_DOUBLE = 0.7;
    constexpr int PROC = 100;
}

class CuckooHashTable {
private:
    class CKNode* table1;
    class CKNode* table2;
    int capacity;
    int size;
    int maxIterations;
    [[nodiscard]] auto hash1(const std::string& key) const -> int;
    [[nodiscard]] auto hash2(const std::string& key) const -> int;
    [[nodiscard]] auto rehash(int newCapacity) -> bool;

public:
    CuckooHashTable(int capacity = Config::DEFAULT_INITIAL_CAP_HASH);
    ~CuckooHashTable();
    [[nodiscard]] auto insert(const std::string& key, std::string& data) -> bool;
    [[nodiscard]] auto search(const std::string& key) const -> std::string;
    [[nodiscard]] auto remove(const std::string& key) -> bool;
    void print() const;
    [[nodiscard]] auto getAllElements() const -> std::vector<std::pair<std::string, std::string>>;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

class DoubleHashTable {
private:
    class DHNode* table;
    int capacity;
    int size;

    [[nodiscard]] auto hash1(const std::string& key) const -> int;
    [[nodiscard]] auto hash2(const std::string& key) const -> int;
    [[nodiscard]] auto rehash(int newCapacity) -> bool;

public:
    DoubleHashTable(int capacity = Config::DEFAULT_INITIAL_CAP_HASH);
    ~DoubleHashTable();
    
    [[nodiscard]] auto insert(const std::string& key, std::string& data) -> bool;
    [[nodiscard]] auto search(const std::string& key) const -> std::string;
    [[nodiscard]] auto remove(const std::string& key) -> bool;
    void print() const;
    [[nodiscard]] auto getAllElements() const -> std::vector<std::pair<std::string, std::string>>;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif