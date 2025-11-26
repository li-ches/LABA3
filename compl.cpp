#include "compl.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>

using namespace std;

CompleteBinaryTree::CompleteBinaryTree() : root(nullptr), treeSize(0) {}

CompleteBinaryTree::~CompleteBinaryTree() {
    clear();
}

void CompleteBinaryTree::insert(int value) {
    TreeNode* newNode = new TreeNode(value);
    if (!root) {
        root = newNode;
        treeSize = 1;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        
        if (!current->left) {
            current->left = newNode;
            break;
        } else {
            q.push(current->left);
        }
        
        if (!current->right) {
            current->right = newNode;
            break;
        } else {
            q.push(current->right);
        }
    }
    treeSize++;
}

bool CompleteBinaryTree::search(int value) const {
    return searchRecursive(root, value);
}

bool CompleteBinaryTree::searchRecursive(TreeNode* node, int value) const {
    if (!node) return false;
    if (node->data == value) return true;
    return searchRecursive(node->left, value) || searchRecursive(node->right, value);
}

void CompleteBinaryTree::remove(int value) {
    if (!root) {
        cout << "Нельзя удалить из пустого дерева." << endl;
        return;
    }
    
    TreeNode* target = nullptr;
    TreeNode* lastNode = nullptr;
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        
        if (current->data == value) {
            target = current;
        }
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
        
        lastNode = current;
    }
    
    if (!target) {
        cout << "Элемент " << value << " не найден." << endl;
        return;
    }
    
    if (target == lastNode) {
        delete target;
        root = nullptr;
        treeSize = 0;
        return;
    }
    
    target->data = lastNode->data;
    
    queue<TreeNode*> q2;
    q2.push(root);
    while (!q2.empty()) {
        TreeNode* current = q2.front();
        q2.pop();
        
        if (current->left == lastNode) {
            current->left = nullptr;
            delete lastNode;
            break;
        } else if (current->left) {
            q2.push(current->left);
        }
        
        if (current->right == lastNode) {
            current->right = nullptr;
            delete lastNode;
            break;
        } else if (current->right) {
            q2.push(current->right);
        }
    }
    treeSize--;
}

void CompleteBinaryTree::print() const {
    if (!root) {
        cout << "Дерево пусто." << endl;
        return;
    }
    
    int levels = log2(treeSize) + 1;
    printRecursive(root, 0, levels);
}

void CompleteBinaryTree::printRecursive(TreeNode* node, int level, int maxLevel) const {
    if (!node || level >= maxLevel) return;
    
    printRecursive(node->right, level + 1, maxLevel);
    
    for (int i = 0; i < level; ++i) {
        cout << "    ";
    }
    cout << node->data << endl;
    
    printRecursive(node->left, level + 1, maxLevel);
}

void CompleteBinaryTree::clear() {
    clearRecursive(root);
    root = nullptr;
    treeSize = 0;
}

void CompleteBinaryTree::clearRecursive(TreeNode* node) {
    if (!node) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}

bool CompleteBinaryTree::isEmpty() const {
    return root == nullptr;
}

int CompleteBinaryTree::getSize() const {
    return treeSize;
}

int CompleteBinaryTree::countTreeSize(TreeNode* node) const {
    if (!node) return 0;
    return 1 + countTreeSize(node->left) + countTreeSize(node->right);
}

void CompleteBinaryTree::setRoot(TreeNode* newRoot) {
    clear();
    root = newRoot;
    treeSize = countTreeSize(newRoot);
}