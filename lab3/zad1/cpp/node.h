#ifndef NODE_H
#define NODE_H

#include <string>
#include <utility>
#include <cstdint>
class OneListNode {
public: 
    std::string data;
    OneListNode* next = nullptr;
    
    explicit OneListNode(std::string val) : data(std::move(val)) {}
};

class TwoListNode {
public: 
    std::string data;
    TwoListNode* next = nullptr;
    TwoListNode* prev = nullptr;
    
    explicit TwoListNode(std::string val) : data(std::move(val)) {}
};

class StackNode {
public:
    std::string data;
    StackNode* next = nullptr;
    
    explicit StackNode(std::string val) : data(std::move(val)) {}
};

class QueueNode {
public:
    std::string data;
    QueueNode* next = nullptr;
    
    explicit QueueNode(std::string val) : data(std::move(val)) {}
};

enum Color : std::uint8_t { RED, BLACK };

class RBTreeNode {
public:
    int data;
    Color color = RED;
    RBTreeNode* left = nullptr;
    RBTreeNode* right = nullptr;
    RBTreeNode* parent = nullptr;
    
    explicit RBTreeNode(int val = 0) : data(val) {}
};

class DHNode {
public:
    std::string key;
    std::string data;
    bool isDeleted = false;
    bool isEmpty = true;
    
    DHNode() = default; 
};

class CKNode {
public:
    std::string key;
    std::string data;
    bool isDeleted = false;
    bool isEmpty = true;
    
    CKNode() = default;
};

#endif