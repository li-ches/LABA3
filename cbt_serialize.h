#ifndef CBT_SERIALIZE_H
#define CBT_SERIALIZE_H

#include "compl.h"
#include <string>

class CbtSerializer {
private:
    static void serializeTree(std::ostream& os, CompleteBinaryTree::TreeNode* node);
    static CompleteBinaryTree::TreeNode* deserializeTree(std::istream& is);
    static void serializeTreeBinary(std::ostream& os, CompleteBinaryTree::TreeNode* node);
    static CompleteBinaryTree::TreeNode* deserializeTreeBinary(std::istream& is);

public:
    static void saveToFile(const CompleteBinaryTree& tree, const std::string& filename);
    static void loadFromFile(CompleteBinaryTree& tree, const std::string& filename);
    static void saveToBinaryFile(const CompleteBinaryTree& tree, const std::string& filename);
    static void loadFromBinaryFile(CompleteBinaryTree& tree, const std::string& filename);
};

#endif