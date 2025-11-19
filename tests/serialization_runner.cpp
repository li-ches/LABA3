#include <iostream>
#include <fstream>
#include <cassert>

#include "../arr.h"
#include "../list.h"
#include "../dList.h"
#include "../stack.h"
#include "../queue.h"
#include "../hash.h"
#include "../compl.h"

using namespace std;

// ==================================================
// ===============     MyArr     ====================
// ==================================================

void testMyArr() {
    cout << "=== Тестирование MyArr ===" << endl;

    MyArr a;
    a.addEnd("Москва");
    a.addEnd("Лондон");
    a.addEnd("Токио");

    a.saveToFile("arr_test.txt");

    MyArr b;
    b.loadFromFile("arr_test.txt");

    assert(b.getAt(0) == "Москва");
    assert(b.getAt(1) == "Лондон");
    assert(b.getAt(2) == "Токио");

    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;

    // binary
    a.saveToFile("arr_test.bin");
    MyArr c;
    c.loadFromFile("arr_test.bin");

    assert(c.getAt(0) == "Москва");
    assert(c.getAt(1) == "Лондон");

    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    cout << "MyArr: OK\n" << endl;
}

// ==================================================
// ===============     MyList     ===================
// ==================================================

void testMyList() {
    cout << "=== Тестирование MyList ===" << endl;

    MyList L;
    L.addTail("A");
    L.addTail("B");
    L.addTail("C");

    L.saveToFile("list_test.txt");

    MyList L2;
    L2.loadFromFile("list_test.txt");

    assert(L2.findValue("A"));
    assert(L2.findValue("B"));
    assert(L2.findValue("C"));

    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;

    L.saveToFile("list_test.bin");
    MyList L3;
    L3.loadFromFile("list_test.bin");

    assert(L3.findValue("A"));
    assert(L3.findValue("C"));

    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    cout << "MyList: OK\n" << endl;
}

// ==================================================
// ===============      DList     ===================
// ==================================================

void testDList() {
    cout << "=== Тестирование DList ===" << endl;

    DList L;
    L.addTail("X");
    L.addTail("Y");

    L.saveToFile("dlist_test.txt");

    DList L2;
    L2.loadFromFile("dlist_test.txt");

    assert(L2.contains("X"));
    assert(L2.contains("Y"));

    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;

    L.saveToFile("dlist_test.bin");
    DList L3;
    L3.loadFromFile("dlist_test.bin");

    assert(L3.contains("X"));

    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    cout << "DList: OK\n" << endl;
}

// ==================================================
// ===============      Stack     ===================
// ==================================================

void testStack() {
    cout << "=== Тестирование Stack ===" << endl;

    Stack S;
    S.push("one");
    S.push("two");

    S.saveToFile("stack_test.txt");

    Stack S2;
    S2.loadFromFile("stack_test.txt");

    assert(S2.pop() == "two");
    assert(S2.pop() == "one");

    cout << "Текстовая: ПРОЙДЕНА\n";

    S.saveToFile("stack_test.bin");
    Stack S3;
    S3.loadFromFile("stack_test.bin");

    assert(S3.pop() == "two");

    cout << "Бинарная: ПРОЙДЕНА\n";
    cout << "Stack: OK\n" << endl;
}

// ==================================================
// ===============      Queue     ===================
// ==================================================

void testQueue() {
    cout << "=== Тестирование Queue ===" << endl;

    Queue Q;
    Q.push("A");
    Q.push("B");

    Q.saveToFile("queue_test.txt");

    Queue Q2;
    Q2.loadFromFile("queue_test.txt");

    assert(Q2.pop() == "A");
    assert(Q2.pop() == "B");

    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;

    Q.saveToFile("queue_test.bin");
    Queue Q3;
    Q3.loadFromFile("queue_test.bin");

    assert(Q3.pop() == "A");

    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    cout << "Queue: OK\n" << endl;
}

// ==================================================
// =======   CompleteBinaryTree (compl)   ===========
// ==================================================

void testCompleteBinaryTree() {
    cout << "=== Тестирование CompleteBinaryTree ===" << endl;

    CompleteBinaryTree T;
    T.insert(50);
    T.insert(30);
    T.insert(70);
    T.insert(20);
    T.insert(40);

    ofstream tf("tree_test.txt");
    T.saveToFile(tf);
    tf.close();

    CompleteBinaryTree T2;
    ifstream tin("tree_test.txt");
    T2.loadFromFile(tin);
    tin.close();

    assert(T2.search(50));
    assert(T2.search(30));
    assert(!T2.search(999));

    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;

    ofstream bf("tree_test.bin", ios::binary);
    T.saveToBinaryFile(bf);
    bf.close();

    CompleteBinaryTree T3;
    ifstream bin("tree_test.bin", ios::binary);
    T3.loadFromBinaryFile(bin);

    assert(T3.search(70));
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;

    cout << "CompleteBinaryTree: OK\n" << endl;
}

// ==================================================
// ========= HashTables (ChainHash/OpenHash) =========
// ==================================================

void testHashTables() {
    cout << "=== Тест Hash (Chain/Open) ===" << endl;

    ChainHash C(5);
    C.insert("A","1");
    C.insert("B","2");

    assert(C.find("A") == "1");
    assert(C.find("NO") == "");

    cout << "ChainHash: OK\n";

    OpenHash H(5);
    H.insert("A","1");
    H.insert("B","2");

    assert(H.find("A") == "1");
    assert(H.find("Z") == "");

    cout << "OpenHash: OK\n" << endl;
}



int main() {
    cout << "ЗАПУСК ВСЕХ ТЕСТОВ\n\n";

    testMyArr();
    testMyList();
    testDList();
    testStack();
    testQueue();
    testCompleteBinaryTree();
    testHashTables();

    cout << "=============================================\n";
    cout << "ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n";
    cout << "=============================================\n";

    return 0;
}
