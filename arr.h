#ifndef ARR_H
#define ARR_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class ArrSerializer;

class MyArr {
private:
    string* data;
    int size;
    int capacity;

    void ensureCapacity(int newSize);

public:
    MyArr();
    ~MyArr();

    MyArr(const MyArr&) = delete;
    MyArr& operator=(const MyArr&) = delete;
    MyArr(MyArr&&) = delete;
    MyArr& operator=(MyArr&&) = delete;

    void addHead(const string& val);
    void delHead();
    void delEnd();

    void addEnd(const string& val);
    string getAt(int idx) const;
    void delAt(int idx);
    void readArray() const;
    void addAt(int idx, const string& val);
    void repArr(int idx, const string& val);

    auto lenArr() const -> int;

    string* getData_test() const { return data; }

private:
    friend class ArrSerializer;
};

#endif