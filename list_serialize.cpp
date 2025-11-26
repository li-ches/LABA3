#include "list_serialize.h"
#include "serialize.h"
#include <iostream>

using namespace std;

void ListSerializer::saveToFile(const MyList& list, const string& filename) {
    ofstream file(filename);
    if (!file) return;

    int count = 0;
    for (MyList::Node* curr = list.head; curr; curr = curr->next) {
        count++;
    }
    file << count << "\n";

    for (MyList::Node* curr = list.head; curr; curr = curr->next) {
        writeStringText(file, curr->value);
    }
}

void ListSerializer::loadFromFile(MyList& list, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    while (list.head) list.delHead();

    int count;
    file >> count;
    string dummy; getline(file, dummy);

    if (file.fail()) return;

    for (int i = 0; i < count; ++i) {
        string val = readStringText(file);
        list.addTail(val);
    }
}

void ListSerializer::saveToBinaryFile(const MyList& list, const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file) return;

    int count = 0;
    for (MyList::Node* curr = list.head; curr; curr = curr->next) {
        count++;
    }
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (MyList::Node* curr = list.head; curr; curr = curr->next) {
        writeString(file, curr->value);
    }
}

void ListSerializer::loadFromBinaryFile(MyList& list, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;

    while (list.head) list.delHead();

    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (file.fail()) return;

    for (int i = 0; i < count; ++i) {
        string val = readString(file);
        if (file.fail()) break;
        list.addTail(val);
    }
}