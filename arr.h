#ifndef ARR_H
#define ARR_H

#include <string>
#include <iostream>
#include <fstream> 

using namespace std;

class MyArr {
private:
    string* data;
    int size;
    int capacity;

    void ensureCapacity(int newSize);

public:
    MyArr();
    ~MyArr();

    // Rule of Five: Запрещаем копирование и перемещение
    MyArr(const MyArr&) = delete;
    MyArr& operator=(const MyArr&) = delete;
    MyArr(MyArr&&) = delete;
    MyArr& operator=(MyArr&&) = delete;

    void addHead(const string& val); // добавление в начало
    void delHead();                   // удаление из начала
    void delEnd();                    // удаление из конца

    void addEnd(const string& val);
    string getAt(int idx) const;
    void delAt(int idx);
    void readArray() const;
    void addAt(int idx, const string& val);
    void repArr(int idx, const string& val);
    
    // Изменено согласно modernize-use-trailing-return-type
    auto lenArr() const -> int;

    //сериализация
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);

    string* getData_test() const //хелпер для тестов
    {   return data;
    }
};

#endif