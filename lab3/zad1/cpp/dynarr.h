#ifndef DYNARR_H
#define DYNARR_H

#include <string>
#include <fstream>

namespace Config {
    constexpr int DEFAULT_INITIAL_CAPACITY = 10;
}

class DynArr {
private:
    std::string* data;
    int cap;
    int len;
public:
    
    DynArr(int initialcap = Config::DEFAULT_INITIAL_CAPACITY);
    ~DynArr();
    
    void addToEnd(const std::string& val);
    void addAtIndex(int index, const std::string& val);
    [[nodiscard]] auto getElement(int index) const -> std::string;
    void replaceElement(int index, const std::string& value) const;
    void removeAtIndex(int index);
    void printArr() const;
    [[nodiscard]] auto getLength() const -> int { return len; }
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif