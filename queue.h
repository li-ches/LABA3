#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class QueueSerializer;

class Queue {
private:
    class QNode {
    public:
        string value;
        QNode* next;
    };

    QNode* head;
    QNode* tail;

public:
    Queue();
    ~Queue();

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    void push(string val);
    string pop();
    string peek() const;
    void print() const;
    bool isEmpty() const;

    QNode* getHead_test() const { return head; }
    QNode* getTail_test() const { return tail; }

private:
    friend class QueueSerializer;
};

#endif