#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <fstream>

class Queue {
private:
    class QueueNode* front;
    class QueueNode* rear;
public:
    
    Queue();
    ~Queue();
    void enqueue(const std::string& val);
    void dequeue();
    void print() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif