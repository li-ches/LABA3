#include <iostream>
#include <fstream>
#include <cassert>
#include "../../zad1/cpp/dynarr.h"
#include "../../zad1/cpp/onelist.h"
#include "../../zad1/cpp/twolist.h"
#include "../../zad1/cpp/stack.h"
#include "../../zad1/cpp/queue.h"
#include "../../zad1/cpp/rbtree.h"
#include "../../zad1/cpp/hash-table.h"

using namespace std;

void testDynArr() {
    cout << "=== Тестирование DynArr ===" << endl;
    
    DynArr arr1;
    arr1.addToEnd("Москва");
    arr1.addToEnd("Лондон");
    arr1.addToEnd("Токио");
    arr1.addToEnd("Париж");
    
    ofstream textFile("dynarr_test.txt");
    arr1.saveToFile(textFile);
    textFile.close();
    
    DynArr arr2;
    ifstream textFileIn("dynarr_test.txt");
    arr2.loadFromFile(textFileIn);
    textFileIn.close();
    
    assert(arr2.getElement(0) == "Москва");
    assert(arr2.getElement(1) == "Лондон");
    assert(arr2.getElement(2) == "Токио");
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("dynarr_test.bin", ios::binary);
    arr1.saveToBinaryFile(binFile);
    binFile.close();
    
    DynArr arr3;
    ifstream binFileIn("dynarr_test.bin", ios::binary);
    arr3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    assert(arr3.getElement(0) == "Москва");
    assert(arr3.getElement(1) == "Лондон");
    assert(arr3.getElement(2) == "Токио");
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "DynArr: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testOneList() {
    cout << "=== Тестирование OneList ===" << endl;
    
    OneList list1;
    list1.addToTail("Берлин");
    list1.addToTail("Мадрид");
    list1.addToTail("Рим");
    
    ofstream textFile("onelist_test.txt");
    list1.saveToFile(textFile);
    textFile.close();
    
    OneList list2;
    ifstream textFileIn("onelist_test.txt");
    list2.loadFromFile(textFileIn);
    textFileIn.close();
    
    assert(list2.getElementByValue("Берлин") == "Берлин");
    assert(list2.getElementByValue("Мадрид") == "Мадрид");
    assert(list2.getElementByValue("Рим") == "Рим");
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("onelist_test.bin", ios::binary);
    list1.saveToBinaryFile(binFile);
    binFile.close();
    
    OneList list3;
    ifstream binFileIn("onelist_test.bin", ios::binary);
    list3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    assert(list3.getElementByValue("Берлин") == "Берлин");
    assert(list3.getElementByValue("Мадрид") == "Мадрид");
    assert(list3.getElementByValue("Рим") == "Рим");
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "OneList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testTwoList() {
    cout << "=== Тестирование TwoList ===" << endl;
    
    TwoList list1;
    list1.addToTail("Пекин");
    list1.addToTail("Сеул");
    list1.addToTail("Бангкок");
    
    ofstream textFile("twolist_test.txt");
    list1.saveToFile(textFile);
    textFile.close();
    
    TwoList list2;
    ifstream textFileIn("twolist_test.txt");
    list2.loadFromFile(textFileIn);
    textFileIn.close();
    
    assert(list2.getElementByValue("Пекин") == "Пекин");
    assert(list2.getElementByValue("Сеул") == "Сеул");
    assert(list2.getElementByValue("Бангкок") == "Бангкок");
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("twolist_test.bin", ios::binary);
    list1.saveToBinaryFile(binFile);
    binFile.close();
    
    TwoList list3;
    ifstream binFileIn("twolist_test.bin", ios::binary);
    list3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    assert(list3.getElementByValue("Пекин") == "Пекин");
    assert(list3.getElementByValue("Сеул") == "Сеул");
    assert(list3.getElementByValue("Бангкок") == "Бангкок");
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "TwoList: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testStack() {
    cout << "=== Тестирование Stack ===" << endl;
    
    Stack stack1;
    stack1.push("Осло");
    stack1.push("Стокгольм");
    stack1.push("Копенгаген");
    
    ofstream textFile("stack_test.txt");
    stack1.saveToFile(textFile);
    textFile.close();
    
    Stack stack2;
    ifstream textFileIn("stack_test.txt");
    stack2.loadFromFile(textFileIn);
    textFileIn.close();
    
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("stack_test.bin", ios::binary);
    stack1.saveToBinaryFile(binFile);
    binFile.close();
    
    Stack stack3;
    ifstream binFileIn("stack_test.bin", ios::binary);
    stack3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "Stack: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testQueue() {
    cout << "=== Тестирование Queue ===" << endl;
    
    Queue queue1;
    queue1.enqueue("Дели");
    queue1.enqueue("Мумбаи");
    queue1.enqueue("Калькутта");
    
    ofstream textFile("queue_test.txt");
    queue1.saveToFile(textFile);
    textFile.close();
    
    Queue queue2;
    ifstream textFileIn("queue_test.txt");
    queue2.loadFromFile(textFileIn);
    textFileIn.close();
    
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("queue_test.bin", ios::binary);
    queue1.saveToBinaryFile(binFile);
    binFile.close();
    
    Queue queue3;
    ifstream binFileIn("queue_test.bin", ios::binary);
    queue3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "Queue: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testRBTree() {
    cout << "=== Тестирование RBTree ===" << endl;
    
    RBTree tree1;
    tree1.insert(50);
    tree1.insert(30);
    tree1.insert(70);
    tree1.insert(20);
    tree1.insert(40);
    
    ofstream textFile("rbtree_test.txt");
    tree1.saveToFile(textFile);
    textFile.close();
    
    RBTree tree2;
    ifstream textFileIn("rbtree_test.txt");
    tree2.loadFromFile(textFileIn);
    textFileIn.close();
    
    assert(tree2.find(50) != tree2.nil);
    assert(tree2.find(30) != tree2.nil);
    assert(tree2.find(70) != tree2.nil);
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("rbtree_test.bin", ios::binary);
    tree1.saveToBinaryFile(binFile);
    binFile.close();
    
    RBTree tree3;
    ifstream binFileIn("rbtree_test.bin", ios::binary);
    tree3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    assert(tree3.find(50) != tree3.nil);
    assert(tree3.find(30) != tree3.nil);
    assert(tree3.find(70) != tree3.nil);
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "RBTree: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testCuckooHashTable() {
    cout << "=== Тестирование CuckooHashTable ===" << endl;
    
    CuckooHashTable table1(10);
    CuckooHashTable table2(10);
    string val1 = "Россия";
    string val2 = "Германия";
    string val3 = "Франция";
    
    bool res1 = table1.insert("Москва", val1);
    bool res2 = table1.insert("Берлин", val2);
    bool res3 = table1.insert("Париж", val3);
    
    cout << "Вставка результатов: " << res1 << ", " << res2 << ", " << res3 << endl;
    
    ofstream textFile("cuckoo_test.txt");
    table1.saveToFile(textFile);
    textFile.close();
    
    ifstream textFileF("cuckoo_test.txt");
    table2.loadFromFile(textFileF);
    textFileF.close();
    
    string s1 = table2.search("Москва");
    string s2 = table2.search("Берлин");
    string s3 = table2.search("Париж");
    
    cout << "Поиск результатов: " << s1 << ", " << s2 << ", " << s3 << endl;
    
    assert(s1 == "Россия");
    assert(s2 == "Германия");
    assert(s3 == "Франция");
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("cuckoo_test.bin", ios::binary);
    table1.saveToBinaryFile(binFile);
    binFile.close();
    
    CuckooHashTable table3(10);
    ifstream binFileIn("cuckoo_test.bin", ios::binary);
    table3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    string s4 = table3.search("Москва");
    string s5 = table3.search("Берлин");
    string s6 = table3.search("Париж");
    
    cout << "Поиск результатов (бинарный): " << s4 << ", " << s5 << ", " << s6 << endl;
    
    assert(s4 == "Россия");
    assert(s5 == "Германия");
    assert(s6 == "Франция");
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "CuckooHashTable: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

void testDoubleHashTable() {
    cout << "=== Тестирование DoubleHashTable ===" << endl;
    
    DoubleHashTable table1(10);
    string val1 = "Япония";
    string val2 = "Корея";
    string val3 = "Китай";
    
    bool res1 = table1.insert("Токио", val1);
    bool res2 = table1.insert("Сеул", val2);
    bool res3 = table1.insert("Пекин", val3);
    
    cout << "Вставка результатов: " << res1 << ", " << res2 << ", " << res3 << endl;
    
    ofstream textFile("double_test.txt");
    table1.saveToFile(textFile);
    textFile.close();
    
    DoubleHashTable table2(10);
    ifstream textFileIn("double_test.txt");
    table2.loadFromFile(textFileIn);
    textFileIn.close();
    
    string s1 = table2.search("Токио");
    string s2 = table2.search("Сеул");
    string s3 = table2.search("Пекин");
    
    cout << "Поиск результатов: " << s1 << ", " << s2 << ", " << s3 << endl;
    
    assert(s1 == "Япония");
    assert(s2 == "Корея");
    assert(s3 == "Китай");
    cout << "Текстовая сериализация: ПРОЙДЕНА" << endl;
    
    ofstream binFile("double_test.bin", ios::binary);
    table1.saveToBinaryFile(binFile);
    binFile.close();
    
    DoubleHashTable table3(10);
    ifstream binFileIn("double_test.bin", ios::binary);
    table3.loadFromBinaryFile(binFileIn);
    binFileIn.close();
    
    string s4 = table3.search("Токио");
    string s5 = table3.search("Сеул");
    string s6 = table3.search("Пекин");
    
    cout << "Поиск результатов (бинарный): " << s4 << ", " << s5 << ", " << s6 << endl;
    
    assert(s4 == "Япония");
    assert(s5 == "Корея");
    assert(s6 == "Китай");
    cout << "Бинарная сериализация: ПРОЙДЕНА" << endl;
    
    cout << "DoubleHashTable: ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n" << endl;
}

int main() {
    cout << "ЗАПУСК ТЕСТОВ СЕРИАЛИЗАЦИИ СТРУКТУР ДАННЫХ\n" << endl;
    
    try {
        testDynArr();
        testOneList();
        testTwoList();
        testStack();
        testQueue();
        testRBTree();
        testCuckooHashTable();
        testDoubleHashTable();
        
        cout << "=========================================" << endl;
        cout << "ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!" << endl;
        cout << "=========================================" << endl;
    } catch (const exception& e) {
        cout << "ОШИБКА: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}