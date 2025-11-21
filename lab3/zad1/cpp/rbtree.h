#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <iostream>
#include <fstream>
#include "node.h"

class RBTree {
private:
    
    void destroyHelper(RBTreeNode* node);
    void leftRotate(RBTreeNode* pivotNode);
    void rightRotate(RBTreeNode* pivotNode);
    void insertFixup(RBTreeNode* newNode);
    void handleRedSiblingCase(RBTreeNode* fixupNode, RBTreeNode* sibling, bool isLeft);
    static void handleBlackChildrenCase(RBTreeNode*& fixupNode, RBTreeNode* sibling);
    void handleMixedChildrenCase(RBTreeNode*& fixupNode, RBTreeNode* sibling, bool isLeft);
    void handleBlackInnerChildCase(RBTreeNode* sibling, RBTreeNode* outerChild, bool isLeft);
    void performFinalFixup(RBTreeNode*& fixupNode, RBTreeNode* sibling, bool isLeft);
    void handleFixupCase(RBTreeNode*& fixupNode);
    void deleteFixup(RBTreeNode* fixupNode);
    void transplant(RBTreeNode* deleteNode, RBTreeNode* childNode);
    void printHelper(RBTreeNode* node, const std::string& prefix = "", bool isLeft = true) const;
    void saveTreeHelper(std::ofstream& file, RBTreeNode* node) const;
    auto loadTreeHelper(std::ifstream& file, RBTreeNode* parent) -> RBTreeNode*;
    void saveToBinaryHelper(std::ofstream& file, RBTreeNode* node) const;
    auto loadFromBinaryHelper(std::ifstream& file, RBTreeNode* parent) -> RBTreeNode*;

public:
    RBTreeNode* root;
    RBTreeNode* nil;
    RBTree();
    ~RBTree();
    void insert(int val);
    void remove(int val);
    [[nodiscard]] auto find(int val) const -> RBTreeNode*;
    void print() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void saveToBinaryFile(std::ofstream& file) const;
    void loadFromBinaryFile(std::ifstream& file);
};

#endif