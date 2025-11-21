#include "gtest/gtest.h"
#include "../compl.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

class OutputCapture {
    std::stringstream buffer;
    std::streambuf* old;
public:
    OutputCapture() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(old); }
    std::string str() const { return buffer.str(); }
};

TEST(TreeTest, EmptyOps) {
    CompleteBinaryTree t;
    // Поиск в пустом
    EXPECT_FALSE(t.search(10));
    // Удаление из пустого
    t.remove(5);
    EXPECT_EQ(t.getSize(), 0);
    // Печать пустого
    OutputCapture cap;
    t.print();
}

TEST(TreeTest, Insert_And_Duplicates) {
    CompleteBinaryTree t;
    t.insert(10);
    EXPECT_TRUE(t.search(10));
    
    // Попытка вставить дубликат (должна игнорироваться)
    int sizeBefore = t.getSize();
    t.insert(10);
    EXPECT_EQ(t.getSize(), sizeBefore);
    
    t.insert(5);  // Left
    t.insert(15); // Right
    EXPECT_TRUE(t.search(5));
    EXPECT_TRUE(t.search(15));
}

TEST(TreeTest, Remove_Scenarios) {
    CompleteBinaryTree t;
    // Строим дерево:
    //      10
    //     /  \
    //    5    15
    //   / \   / \
    //  2   7 12 18
    t.insert(10); t.insert(5); t.insert(15);
    t.insert(2); t.insert(7); t.insert(12); t.insert(18);

    // 1. Удаление листа (2)
    t.remove(2);
    EXPECT_FALSE(t.search(2));
    
    // 2. Удаление узла с одним ребенком
    // Удалим 7, добавим 6 (чтобы у 5 был ребенок) - упростим тест
    // Пересоздадим для точности структуры
    t.clear();
    t.insert(10); t.insert(5); t.insert(3); // 5 имеет левого 3
    t.remove(5); // Удаляем 5, 3 должен подтянуться
    EXPECT_FALSE(t.search(5));
    EXPECT_TRUE(t.search(3));

    t.clear();
    t.insert(10); t.insert(5); t.insert(7); // 5 имеет правого 7
    t.remove(5);
    EXPECT_FALSE(t.search(5));
    EXPECT_TRUE(t.search(7));

    // 3. Удаление узла с ДВУМЯ детьми (root 10)
    t.clear();
    t.insert(10); t.insert(5); t.insert(15);
    t.remove(10); 
    EXPECT_FALSE(t.search(10));
    EXPECT_TRUE(t.search(15)); // 15 или 5 встанет на место (зависит от реализации findMin)
    EXPECT_EQ(t.getSize(), 2);
    
    // 4. Удаление несуществующего
    t.remove(999);
    EXPECT_EQ(t.getSize(), 2);
}

TEST(TreeTest, IO_Errors) {
    CompleteBinaryTree t;
    t.loadFromFile("missing.txt");
    t.loadFromBinaryFile("missing.bin");
    
    t.saveToFile(""); // Fail open
    t.saveToBinaryFile(""); // Fail open

    // Corrupt read
    { ofstream f("bad.bin", ios::binary); int m=-1; f.write((char*)&m, 4); }
    t.loadFromBinaryFile("bad.bin");
    remove("bad.bin");
}

TEST(TreeTest, SaveLoad_Happy) {
    CompleteBinaryTree t;
    t.insert(10); t.insert(20);
    
    t.saveToFile("tree.txt");
    CompleteBinaryTree t2;
    t2.loadFromFile("tree.txt");
    EXPECT_TRUE(t2.search(10));
    remove("tree.txt");

    t.saveToBinaryFile("tree.bin");
    CompleteBinaryTree t3;
    t3.loadFromBinaryFile("tree.bin");
    EXPECT_TRUE(t3.search(20));
    remove("tree.bin");
}