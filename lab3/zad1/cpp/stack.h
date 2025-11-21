#ifndef STACK_H
#define STACK_H

#include <string>
#include <fstream>

class Stack {
private:
    class StackNode* top;
public:
    
    Stack();
    ~Stack();
    void push(const std::string& val);
    void pop();
    void print() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif