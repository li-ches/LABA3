#ifndef COMPL_H
#define COMPL_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class CbtSerializer;

class CompleteBinaryTree {
public:
    struct TreeNode {
        int data;
        TreeNode* left;
        TreeNode* right;
        
        TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
    };

private:
    TreeNode* root;
    int treeSize;

    bool searchRecursive(TreeNode* node, int value) const;
    void printRecursive(TreeNode* node, int level, int maxLevel) const;
    void clearRecursive(TreeNode* node);
    int countTreeSize(TreeNode* node) const;

public:
    CompleteBinaryTree();
    ~CompleteBinaryTree();

    CompleteBinaryTree(const CompleteBinaryTree&) = delete;
    CompleteBinaryTree& operator=(const CompleteBinaryTree&) = delete;
    CompleteBinaryTree(CompleteBinaryTree&&) = delete;
    CompleteBinaryTree& operator=(CompleteBinaryTree&&) = delete;

    void insert(int value);
    bool search(int value) const;
    void remove(int value);
    void print() const;
    void clear();
    bool isEmpty() const;
    int getSize() const;

    TreeNode* getRoot_test() const { return root; }
    void setRoot(TreeNode* newRoot);

private:
    friend class CbtSerializer;
};

#endif