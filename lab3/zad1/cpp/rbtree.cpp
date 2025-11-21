#include <iostream>
#include <fstream>
#include "node.h"
#include "rbtree.h"

using namespace std;

RBTree::RBTree() : nil(new RBTreeNode())
{
    nil->color = BLACK;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
}

RBTree::~RBTree() {
    if (root != nil) {
        destroyHelper(root);
    }
    delete nil;
}

void RBTree::destroyHelper(RBTreeNode* node) {
    if (node == nil) {
        return;
    }
    if (node->left != nil) {
        destroyHelper(node->left);
    }
    if (node->right != nil) {
        destroyHelper(node->right);
    }
    delete node;
}

void RBTree::leftRotate(RBTreeNode* pivotNode) {
    RBTreeNode* childNode = pivotNode->right;
    pivotNode->right = childNode->left;
    if (childNode->left != nil) {
        childNode->left->parent = pivotNode;
    }
    childNode->parent = pivotNode->parent;
    if (pivotNode->parent == nil) {
        root = childNode;
    } else if (pivotNode == pivotNode->parent->left) {
        pivotNode->parent->left = childNode;
    } else {
        pivotNode->parent->right = childNode;
    }
    childNode->left = pivotNode;
    pivotNode->parent = childNode;
}

void RBTree::rightRotate(RBTreeNode* pivotNode) {
    RBTreeNode* childNode = pivotNode->left;
    pivotNode->left = childNode->right;
    if (childNode->right != nil) {
        childNode->right->parent = pivotNode;
    }
    childNode->parent = pivotNode->parent;
    if (pivotNode->parent == nil) {
        root = childNode;
    } else if (pivotNode == pivotNode->parent->left) {
        pivotNode->parent->left = childNode;
    } else {
        pivotNode->parent->right = childNode;
    }
    childNode->right = pivotNode;
    pivotNode->parent = childNode;
}

void RBTree::insertFixup(RBTreeNode* newNode) {
    while (newNode->parent->color == RED) {
        if (newNode->parent == newNode->parent->parent->left) {
            RBTreeNode* uncleNode = newNode->parent->parent->right;
            if (uncleNode->color == RED) {
                newNode->parent->color = BLACK;
                uncleNode->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            } else {
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    leftRotate(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                rightRotate(newNode->parent->parent);
            }
        } else {
            RBTreeNode* uncleNode = newNode->parent->parent->left;
            if (uncleNode->color == RED) {
                newNode->parent->color = BLACK;
                uncleNode->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            } else {
                if (newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rightRotate(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                leftRotate(newNode->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RBTree::insert(int val) {
    auto *newNode = new RBTreeNode(val);
    newNode->color = RED;
    newNode->left = nil;
    newNode->right = nil;
    newNode->parent = nil;
    
    RBTreeNode* parentNode = nil;
    RBTreeNode* currentNode = root;
    
    while (currentNode != nil) {
        parentNode = currentNode;
        if (newNode->data < currentNode->data) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    
    newNode->parent = parentNode;
    
    if (parentNode == nil) {
        root = newNode;
    } else if (newNode->data < parentNode->data) {
        parentNode->left = newNode;
    } else {
        parentNode->right = newNode;
    }
    
    insertFixup(newNode);
}

void RBTree::handleRedSiblingCase(RBTreeNode* fixupNode, RBTreeNode* sibling, bool isLeft) {
    sibling->color = BLACK;
    fixupNode->parent->color = RED;
    
    if (isLeft) {
        leftRotate(fixupNode->parent);
    } else {
        rightRotate(fixupNode->parent);
    }
}

void RBTree::handleBlackChildrenCase(RBTreeNode*& fixupNode, RBTreeNode* sibling) {
    sibling->color = RED;
    fixupNode = fixupNode->parent;
}

void RBTree::handleMixedChildrenCase(RBTreeNode*& fixupNode, RBTreeNode* sibling, bool isLeft) {
    RBTreeNode* innerChild = isLeft ? sibling->left : sibling->right;
    RBTreeNode* outerChild = isLeft ? sibling->right : sibling->left;
    
    if (innerChild->color == BLACK) {
        handleBlackInnerChildCase(sibling, outerChild, isLeft);
        sibling = isLeft ? fixupNode->parent->right : fixupNode->parent->left;
    }
    
    performFinalFixup(fixupNode, sibling, isLeft);
}

void RBTree::handleBlackInnerChildCase(RBTreeNode* sibling, RBTreeNode* outerChild, bool isLeft) {
    outerChild->color = BLACK;
    sibling->color = RED;
    
    if (isLeft) {
        rightRotate(sibling);
    } else {
        leftRotate(sibling);
    }
}

void RBTree::performFinalFixup(RBTreeNode*& fixupNode, RBTreeNode* sibling, bool isLeft) {
    sibling->color = fixupNode->parent->color;
    fixupNode->parent->color = BLACK;
    
    RBTreeNode* outerChild = isLeft ? sibling->right : sibling->left;
    outerChild->color = BLACK;
    
    if (isLeft) {
        leftRotate(fixupNode->parent);
    } else {
        rightRotate(fixupNode->parent);
    }
    
    fixupNode = root;
}

void RBTree::handleFixupCase(RBTreeNode*& fixupNode) {
    bool isLeft = (fixupNode == fixupNode->parent->left);
    RBTreeNode* sibling = isLeft ? fixupNode->parent->right : fixupNode->parent->left;
    
    if (sibling->color == RED) {
        handleRedSiblingCase(fixupNode, sibling, isLeft);
        sibling = isLeft ? fixupNode->parent->right : fixupNode->parent->left;
    }
    
    RBTreeNode* siblingLeft = isLeft ? sibling->left : sibling->right;
    RBTreeNode* siblingRight = isLeft ? sibling->right : sibling->left;
    
    if (siblingLeft->color == BLACK && siblingRight->color == BLACK) {
        handleBlackChildrenCase(fixupNode, sibling);
    } else {
        handleMixedChildrenCase(fixupNode, sibling, isLeft);
    }
}

void RBTree::deleteFixup(RBTreeNode* fixupNode) {
    while (fixupNode != root && fixupNode->color == BLACK) {
        handleFixupCase(fixupNode);
    }
    
    if (fixupNode != nil) {
        fixupNode->color = BLACK;
    }
}

void RBTree::transplant(RBTreeNode* deleteNode, RBTreeNode* childNode) {
    if (deleteNode->parent == nil) {
        root = childNode;
    } else if (deleteNode == deleteNode->parent->left) {
        deleteNode->parent->left = childNode;
    } else {
        deleteNode->parent->right = childNode;
    }
    if (childNode != nil) {
        childNode->parent = deleteNode->parent;
    }
}

void RBTree::remove(int val) {
    RBTreeNode* deleteNode = find(val);
    if (deleteNode == nil) {
        throw runtime_error("error");
    }
    
    RBTreeNode* successorNode = deleteNode;
    RBTreeNode* childNode;
    Color originalColor = successorNode->color;
    
    if (deleteNode->left == nil) {
        childNode = deleteNode->right;
        transplant(deleteNode, childNode);
        if (originalColor == BLACK) {
            deleteFixup(childNode);
        }
        delete deleteNode;
    } else if (deleteNode->right == nil) {
        childNode = deleteNode->left;
        transplant(deleteNode, childNode);
        if (originalColor == BLACK) {
            deleteFixup(childNode);
        }
        delete deleteNode;
    } else {
        successorNode = deleteNode->left;
        while (successorNode->right != nil) {
            successorNode = successorNode->right;
        }
        originalColor = successorNode->color;
        childNode = successorNode->left;
        
        if (successorNode->parent == deleteNode) {
            childNode->parent = successorNode;
        } else {
            transplant(successorNode, childNode);
            successorNode->left = deleteNode->left;
            successorNode->left->parent = successorNode;
        }
        
        transplant(deleteNode, successorNode);
        successorNode->right = deleteNode->right;
        successorNode->right->parent = successorNode;
        successorNode->color = deleteNode->color;
        
        delete deleteNode;
        
        if (originalColor == BLACK && childNode != nil) {
            deleteFixup(childNode);
        }
    }
}

auto RBTree::find(int val) const -> RBTreeNode *
{
    RBTreeNode* currentNode = root;
    while (currentNode != nil && currentNode->data != val) {
        if (val < currentNode->data) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    return currentNode;
}

void RBTree::printHelper(RBTreeNode* node, const string& prefix, bool isLeft) const {
    if (node != nil) {
        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        
        string color_str = (node->color == RED) ? "R" : "B";
        cout << node->data << "(" << color_str << ")" << '\n';

        string newPrefix = prefix + (isLeft ? "│   " : "    ");

        if (node->left != nil || node->right != nil) {
            if (node->right != nil) {
                printHelper(node->right, newPrefix, false);  
            }
            if (node->left != nil) {
                printHelper(node->left, newPrefix, true);
            }
        }
    }
}

void RBTree::print() const {
    if (root == nil) {
        cout << "error" << '\n';
        return;
    }
    printHelper(root);
}

void RBTree::saveTreeHelper(ofstream& file, RBTreeNode* node) const {
    if (node == nil) {
        file << "# "; 
        return;
    }
    string color_str = (node->color == RED) ? "R" : "B";
    file << node->data << color_str << " ";
    saveTreeHelper(file, node->left);
    saveTreeHelper(file, node->right);
}

auto RBTree::loadTreeHelper(ifstream& file, RBTreeNode* parentNode) -> RBTreeNode*
{
    string token;
    if (!(file >> token)) {
        return nil;
    }
    
    if (token == "#") {
        return nil;
    }
    
    size_t position = 0;
    while (position < token.length() && ((isdigit(token[position]) != 0) || token[position] == '-'))
    {
        position++;
    }

    if (position == 0 || position == token.length()) {
        return nil;
    }
    
    string number_str = token.substr(0, position);
    string color_str = token.substr(position, 1);

    auto* newNode = new RBTreeNode;
    newNode->data = stoi(number_str);
    newNode->color = (color_str == "R") ? RED : BLACK;
    newNode->parent = parentNode;
    newNode->left = nil;
    newNode->right = nil;
    
    newNode->left = loadTreeHelper(file, newNode);
    newNode->right = loadTreeHelper(file, newNode);
    
    return newNode;
}

void RBTree::saveToFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    saveTreeHelper(file, root);
}

void RBTree::loadFromFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    destroyHelper(root);
 
    root = loadTreeHelper(file, nil);

    if (root != nil) {
        root->parent = nil;
    }
}

void RBTree::saveToBinaryHelper(ofstream& file, RBTreeNode* node) const {
    if (node == nil) {
        char nodeMarker = 0;
        file.write(&nodeMarker, sizeof(nodeMarker));
        return;
    }
    
    char nodeMarker = 1;
    file.write(&nodeMarker, sizeof(nodeMarker));
    file.write(reinterpret_cast<const char*>(&node->data), sizeof(node->data));
    file.write(reinterpret_cast<const char*>(&node->color), sizeof(node->color));
    
    saveToBinaryHelper(file, node->left);
    saveToBinaryHelper(file, node->right);
}

auto RBTree::loadFromBinaryHelper(ifstream& file, RBTreeNode* parentNode) -> RBTreeNode*
{
    char nodeMarker;
    file.read(&nodeMarker, sizeof(nodeMarker));
    
    if (nodeMarker == 0) {
        return nil;
    }

    auto* newNode = new RBTreeNode;
    file.read(reinterpret_cast<char*>(&newNode->data), sizeof(newNode->data));
    file.read(reinterpret_cast<char*>(&newNode->color), sizeof(newNode->color));
    newNode->parent = parentNode;
    newNode->left = nil;
    newNode->right = nil;
    
    newNode->left = loadFromBinaryHelper(file, newNode);
    newNode->right = loadFromBinaryHelper(file, newNode);
    
    return newNode;
}

void RBTree::saveToBinaryFile(ofstream& file) const {
    if (!file.is_open()) {
        return;
    }
    saveToBinaryHelper(file, root);
}

void RBTree::loadFromBinaryFile(ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    destroyHelper(root);
    
    root = loadFromBinaryHelper(file, nil);
    
    if (root != nil) {
        root->parent = nil;
    }
}