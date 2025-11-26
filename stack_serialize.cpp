#include "stack_serialize.h"
#include "serialize.h"
#include <iostream>

using namespace std;

void StackSerializer::saveToFile(const Stack& stack, const string& filename) {
    ofstream file(filename);
    if (!file) return;

    int count = 0;
    for (Stack::SNode* curr = stack.top; curr; curr = curr->next) {
        count++;
    }
    file << count << "\n";

    Stack::SNode* curr = stack.top;
    string* tempArr = new string[count];
    for (int i = count - 1; i >= 0; --i) {
        tempArr[i] = curr->value;
        curr = curr->next;
    }

    for (int i = 0; i < count; ++i) {
        writeStringText(file, tempArr[i]);
    }
    delete[] tempArr;
}

void StackSerializer::loadFromFile(Stack& stack, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    while (!stack.isEmpty()) stack.pop();

    int count;
    file >> count;
    string dummy; getline(file, dummy);

    for (int i = 0; i < count; ++i) {
        stack.push(readStringText(file));
    }
}

void StackSerializer::saveToBinaryFile(const Stack& stack, const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file) return;

    int count = 0;
    for (Stack::SNode* curr = stack.top; curr; curr = curr->next) {
        count++;
    }
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    Stack::SNode* curr = stack.top;
    string* tempArr = new string[count];
    for (int i = count - 1; i >= 0; --i) {
        tempArr[i] = curr->value;
        curr = curr->next;
    }

    for (int i = 0; i < count; ++i) {
        writeString(file, tempArr[i]);
    }
    delete[] tempArr;
}

void StackSerializer::loadFromBinaryFile(Stack& stack, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;

    while (!stack.isEmpty()) stack.pop();

    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (file.fail()) return;

    for (int i = 0; i < count; ++i) {
        string val = readString(file);
        if (file.fail()) break;
        stack.push(val);
    }
}