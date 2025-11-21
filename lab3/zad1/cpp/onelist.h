#ifndef ONELIST_H
#define ONELIST_H

#include <string>
#include <fstream>

class OneList {
private:
    class OneListNode* head;
    class OneListNode* tail;
public:
    
    OneList();
    ~OneList();
    void addToHead(const std::string& val);
    void addToTail(const std::string& val);
    void addAfterValue(const std::string& targetVal, const std::string& newVal);
    void addBeforeValue(const std::string& targetVal, const std::string& newVal);
    void removeFromHead();
    void removeFromTail();
    void removeAfterValue(const std::string& targetVal);
    void removeBeforeValue(const std::string& targetVal);
    void removeByValue(const std::string& targetVal);
    [[nodiscard]] auto getElementByValue(const std::string &targetVal) const -> std::string;
    [[nodiscard]] auto findByVal(const std::string& val) const -> OneListNode*; 
    void print() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif