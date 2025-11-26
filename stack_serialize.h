#ifndef STACK_SERIALIZE_H
#define STACK_SERIALIZE_H

#include "stack.h"

class StackSerializer {
public:
    static void saveToFile(const Stack& stack, const std::string& filename);
    static void loadFromFile(Stack& stack, const std::string& filename);
    static void saveToBinaryFile(const Stack& stack, const std::string& filename);
    static void loadFromBinaryFile(Stack& stack, const std::string& filename);
};

#endif