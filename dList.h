#ifndef DLIST_H
#define DLIST_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class DList {
private:
    // cтруктура узла двусвязного списка 
    class DNode {
    public:
        string value;   // значение узла
        DNode* next;    // указатель на следующий элемент
        DNode* prev;    // указатель на предыдущий элемент

        // конс
        DNode(const string& v, DNode* n, DNode* p) : value(v), next(n), prev(p) {}
    };

    DNode* head; // указатель на первый элемент
    DNode* tail; // указатель на последний элемент

    // Приватный хелпер для поиска узла (возвращает приватный тип)
    auto findValue(const string& val) const -> DNode*;

public:
    // kонструктор 
    DList();

    // Деструктор 
    ~DList();


    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;
    DList(DList&&) = delete;
    DList& operator=(DList&&) = delete;

    // Добавление в начало
    void addHead(const string& val);

    // Добавление в хвост
    void addTail(const string& val);

    // Вставка до элемента
    void addBefore(const string& target, const string& val);

    // Вставка после элемента
    void addAfter(const string& target, const string& val);

    // Удаление из начала
    void delHead();

    // Удаление из хвоста
    void delTail();

    void delByVal(const string& val);

    // Обновлено: trailing return type
    auto contains(const string& val) const -> bool;

    void readForward() const;

    void readBackward() const;

    void delAfterValue(const string& val);

    void delBeforeValue(const string& val);

    // сериализация
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
  
    DNode* getHead_Test() const
    {
        return head;
    }
    DNode* getTail_Test()const 
    {
        return tail;
    }
};

#endif