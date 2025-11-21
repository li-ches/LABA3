#ifndef TWOLIST_H
#define TWOLIST_H

#include <string>
#include <fstream>

class TwoList {
private:
    class TwoListNode* head;
    class TwoListNode* tail;
public:
    
    TwoList();
    ~TwoList();
    void addToHead(const std::string& val);
    void addToTail(const std::string& val);
    void addAfterValue(const std::string& targetVal, const std::string& newval);
    void addBeforeValue(const std::string& targetVal, const std::string& newval);
    void removeFromHead();
    void removeFromTail();
    void removeAfterValue(const std::string& targetVal);
    void removeBeforeValue(const std::string& targetVal);
    void removeByValue(const std::string& targetVal);
    [[nodiscard]] auto getElementByValue(const std::string &targetVal) const -> std::string;
    [[nodiscard]] auto findByVal(const std::string& val) const -> class TwoListNode*;
    void print() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};
#endif