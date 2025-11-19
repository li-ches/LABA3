#include "gtest/gtest.h"
#include "../list.h"
#include "../serialize.h" // Нужно для создания битых файлов вручную
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio> // для remove

using namespace std;

class OutputCapture {
    std::stringstream buffer;
    std::streambuf* prev;
public:
    OutputCapture() : prev(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(prev); }
    std::string str() { return buffer.str(); }
};

// --- СТАРЫЕ ТЕСТЫ (Логика работы) ---

TEST(MyListTest, Branch_Deletions) {
    MyList list;
    OutputCapture cap;

    list.delTail(); // Empty
    list.addHead("A");
    list.delTail(); // Single
    EXPECT_EQ(list.getHead_test(), nullptr);
    
    list.addTail("A");
    list.addTail("B");
    list.delTail(); // Loop
    EXPECT_EQ(list.getHead_test()->value, "A");

    list.delByValue("Z"); // Not found
    list.delByValue("A"); // Head match
    EXPECT_EQ(list.getHead_test(), nullptr);

    list.addTail("X"); list.addTail("Y");
    list.delByValue("Y"); // Body match
    EXPECT_FALSE(list.findValue("Y"));
}

TEST(MyListTest, Branch_AdvancedOps) {
    MyList list;
    OutputCapture cap;
    
    list.delAfterValue("A"); 
    list.addTail("A");
    list.delAfterValue("A"); 
    list.addTail("B");
    list.delAfterValue("Z"); 
    list.delAfterValue("A"); 
    EXPECT_FALSE(list.findValue("B"));

    list.delBeforeValue("A");
    list.addHead("A");
    list.delBeforeValue("A"); 
    
    MyList l2;
    l2.addTail("1"); l2.addTail("2"); l2.addTail("3");
    l2.delBeforeValue("2"); 
    EXPECT_EQ(l2.getHead_test()->value, "2");

    MyList l3;
    l3.addTail("X"); l3.addTail("Y"); l3.addTail("Z"); l3.addTail("W");
    l3.delBeforeValue("W"); 
    EXPECT_FALSE(l3.findValue("Z"));
    
    l3.readBack();
}

TEST(MyListTest, AddBeforeHeadValue) {
    OutputCapture cap;
    MyList l;
    l.addBefore("A", "B");
    l.addHead("A");
    l.addBefore("A", "Z"); 
    EXPECT_EQ(l.getHead_test()->value, "Z");
}

TEST(MyListTest, AddAfterNotFound) {
    OutputCapture cap;
    MyList l;
    l.addTail("A");
    l.addAfter("Z", "B"); 
}

TEST(MyListTest, DelByValueEmpty) {
    OutputCapture cap;
    MyList l;
    l.delByValue("A"); 
}

TEST(MyListTest, DelTailMultiple) {
    OutputCapture cap;
    MyList l;
    l.addTail("A"); l.addTail("B"); l.addTail("C");
    l.delTail(); 
    EXPECT_TRUE(l.findValue("B"));
    EXPECT_FALSE(l.findValue("C"));
}

TEST(ListTest, BranchBooster_ComplexConditions) {
    OutputCapture cap;
    MyList l;

    l.delAfterValue("A"); 
    l.addHead("A");
    l.delAfterValue("A"); 

    l.addTail("B");
    l.delAfterValue("Z");
    l.delAfterValue("B");
    
    MyList l2;
    l2.delBeforeValue("A"); 
    l2.addHead("A");
    l2.delBeforeValue("A");

    l2.addTail("B");
    l2.delBeforeValue("B"); 
    EXPECT_EQ(l2.getHead_test()->value, "B");

    MyList l3;
    l3.addTail("A"); l3.addTail("B"); l3.addTail("C");
    l3.delBeforeValue("Z");

    MyList l4;
    l4.addTail("A"); l4.addTail("B");
    l4.delBeforeValue("Z");
}

TEST(DestructorTest, MyListCleanup) {
    OutputCapture cap;
    {
        MyList l;
        l.addTail("1");
        l.addTail("2");
    }
}

// --- НОВЫЕ ТЕСТЫ (ДЛЯ ПОКРЫТИЯ > 90%) ---

TEST(MyListTest, Coverage_EmptyPrints) {
    // Покрывает ветки "Список пуст" в readForward/readBack
    MyList l;
    OutputCapture cap;
    
    l.readForward();
    l.readBack();
    
    // Проверяем, что вывод содержит "пуст"
    string out = cap.str();
    EXPECT_TRUE(out.find("пуст") != string::npos);
}

TEST(MyListTest, Coverage_DelHeadEmpty) {
    // Покрывает ветку if(!head) в delHead
    MyList l;
    l.delHead(); 
    EXPECT_EQ(l.getHead_test(), nullptr);
}

TEST(MyListTest, Coverage_Serialization_Fail) {
    MyList l;
    
    // 1. Ошибка открытия файла при записи (пустое имя)
    // Покрывает if (!file) return; в saveToFile
    l.saveToFile(""); 

    // 2. Ошибка открытия файла при чтении
    // Покрывает if (!file) return; в loadFromFile
    l.loadFromFile("non_existent_file_123.dat");
}

TEST(MyListTest, Coverage_Load_Corrupted) {
    // Покрывает ветки if (file.fail()) break; внутри цикла загрузки
    MyList l;
    
    // Создаем "битый" файл: говорим, что там 5 элементов, а пишем 0
    {
        ofstream f("corrupted_list.dat", ios::binary);
        int count = 5;
        f.write((char*)&count, sizeof(count));
        // Данные не пишем
        f.close();
    }
    
    l.loadFromFile("corrupted_list.dat");
    // Список должен быть пуст (или содержать то, что успел считать до ошибки)
    
    remove("corrupted_list.dat");
}

TEST(MyListTest, Coverage_Load_Overwrite) {
    // Покрывает цикл очистки while(head) delHead() внутри loadFromFile
    MyList l;
    l.addTail("Old1");
    l.addTail("Old2");
    
    // Создаем нормальный файл с 1 элементом
    {
        ofstream f("good_list.dat", ios::binary);
        int count = 1;
        f.write((char*)&count, sizeof(count));
        size_t len = 3;
        f.write((char*)&len, sizeof(len));
        f.write("New", 3);
        f.close();
    }
    
    // Загрузка должна сначала удалить Old1 и Old2
    l.loadFromFile("good_list.dat");
    
    EXPECT_EQ(l.getHead_test()->value, "New");
    EXPECT_EQ(l.getHead_test()->next, nullptr);
    
    remove("good_list.dat");
}