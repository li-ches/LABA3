#include "cbt_serialize.h"
#include "serialize.h"
#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Вспомогательная функция для сериализации дерева в текстовом формате
void CbtSerializer::serializeTree(ostream& os, CompleteBinaryTree::TreeNode* node) {
    if (!node) {
        // Для текстового формата используем обычный вывод
        os << "null\n";
        return;
    }
    os << node->data << "\n";
    serializeTree(os, node->left);
    serializeTree(os, node->right);
}

// Вспомогательная функция для десериализации дерева из текстового формата
CompleteBinaryTree::TreeNode* CbtSerializer::deserializeTree(istream& is) {
    string token;
    if (!getline(is, token) || token.empty()) return nullptr;
    
    if (token == "null") return nullptr;
    
    try {
        int value = stoi(token);
        CompleteBinaryTree::TreeNode* node = new CompleteBinaryTree::TreeNode(value);
        node->left = deserializeTree(is);
        node->right = deserializeTree(is);
        return node;
    } catch (...) {
        return nullptr;
    }
}

// Вспомогательная функция для сериализации дерева в бинарном формате
void CbtSerializer::serializeTreeBinary(ostream& os, CompleteBinaryTree::TreeNode* node) {
    if (!node) {
        int marker = -1;
        os.write(reinterpret_cast<const char*>(&marker), sizeof(marker));
        return;
    }
    os.write(reinterpret_cast<const char*>(&node->data), sizeof(node->data));
    serializeTreeBinary(os, node->left);
    serializeTreeBinary(os, node->right);
}

// Вспомогательная функция для десериализации дерева из бинарного формата
CompleteBinaryTree::TreeNode* CbtSerializer::deserializeTreeBinary(istream& is) {
    int value;
    is.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (is.fail() || value == -1) return nullptr;
    
    CompleteBinaryTree::TreeNode* node = new CompleteBinaryTree::TreeNode(value);
    node->left = deserializeTreeBinary(is);
    node->right = deserializeTreeBinary(is);
    return node;
}

void CbtSerializer::saveToFile(const CompleteBinaryTree& tree, const string& filename) {
    ofstream file(filename);
    if (!file) return;
    serializeTree(file, tree.getRoot_test());
}

void CbtSerializer::loadFromFile(CompleteBinaryTree& tree, const string& filename) {
    ifstream file(filename);
    if (!file) return;
    
    // Очищаем существующее дерево
    tree.clear();
    
    // Десериализуем новое дерево
    CompleteBinaryTree::TreeNode* newRoot = deserializeTree(file);
    
    // Устанавливаем новый корень
    tree.setRoot(newRoot);
}

void CbtSerializer::saveToBinaryFile(const CompleteBinaryTree& tree, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file) return;
    serializeTreeBinary(file, tree.getRoot_test());
}

void CbtSerializer::loadFromBinaryFile(CompleteBinaryTree& tree, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;
    
    // Очищаем существующее дерево
    tree.clear();
    
    // Десериализуем новое дерево
    CompleteBinaryTree::TreeNode* newRoot = deserializeTreeBinary(file);
    
    // Устанавливаем новый корень
    tree.setRoot(newRoot);
}