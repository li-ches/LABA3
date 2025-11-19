#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>


// Класс узла дерева
class TreeNode {
public:
    int data;
    TreeNode* left;
    TreeNode* right;

    //конструктор узла
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Основной класс дерева
class CompleteBinaryTree {
private:
    TreeNode* root;
    int size;

    // Вспомогательные приватные методы (скрыты от пользователя)
    void deleteTreeRecursive(TreeNode* node);
    TreeNode* findMin(TreeNode* node);
    TreeNode* deleteNodeRecursive(TreeNode* current, int value);
    void printTreeUtil(TreeNode* node, const std::string& prefix, bool isLast) const;

public:
    // Конструктор и деструктор
    CompleteBinaryTree();
    ~CompleteBinaryTree();

    // Публичные методы интерфейса
    bool isEmpty() const;
    void clear();
    void insert(int value);
    void remove(int value); // Переименовал deleteNodeCBT в remove для краткости
    bool search(int value) const;
    void print() const;
    int getSize() const;

    void saveToFile(std::ofstream& out) const;
void loadFromFile(std::ifstream& in);

void saveToBinaryFile(std::ofstream& out) const;
void loadFromBinaryFile(std::ifstream& in);
};


#endif