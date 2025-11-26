#include "queue_serialize.h"
#include "serialize.h"
#include <iostream>

using namespace std;

void QueueSerializer::saveToFile(const Queue& queue, const string& filename) {
    ofstream file(filename);
    if (!file) return;

    int count = 0;
    for (Queue::QNode* curr = queue.getHead_test(); curr; curr = curr->next) {
        count++;
    }
    file << count << "\n";

    for (Queue::QNode* curr = queue.getHead_test(); curr; curr = curr->next) {
        writeStringText(file, curr->value);
    }
}

void QueueSerializer::loadFromFile(Queue& queue, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    while (!queue.isEmpty()) queue.pop();

    int count;
    file >> count;
    string dummy; getline(file, dummy);

    if (file.fail()) return;

    for (int i = 0; i < count; ++i) {
        string val = readStringText(file);
        queue.push(val);
    }
}

void QueueSerializer::saveToBinaryFile(const Queue& queue, const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (!file) return;

    int count = 0;
    for (Queue::QNode* curr = queue.getHead_test(); curr; curr = curr->next) {
        count++;
    }
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (Queue::QNode* curr = queue.getHead_test(); curr; curr = curr->next) {
        writeString(file, curr->value);
    }
}

void QueueSerializer::loadFromBinaryFile(Queue& queue, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return;

    while (!queue.isEmpty()) queue.pop();

    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (file.fail()) return;

    for (int i = 0; i < count; ++i) {
        string val = readString(file);
        if (file.fail()) break;
        queue.push(val);
    }
}