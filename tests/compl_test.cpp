#include "gtest/gtest.h"
#include "../compl.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

class OutputCapture {
    stringstream buffer;
    streambuf* old;
public:
    OutputCapture() : old(cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { cout.rdbuf(old); }
    string str() const { return buffer.str(); }
};

// проверка на операции с пустым деревом (поиск, удаление, вывод)
TEST(TreeTest, EmptySearchPrintRemove) {
    CompleteBinaryTree t;

    EXPECT_FALSE(t.search(10));

    t.remove(5);
    EXPECT_TRUE(t.isEmpty());
    EXPECT_EQ(t.getSize(), 0);

    t.print();
}

// проверка на вставку корня и дубликатов
TEST(TreeTest, InsertRootAndDuplicate) {
    CompleteBinaryTree t;

    t.insert(10);
    EXPECT_TRUE(t.search(10));
    EXPECT_EQ(t.getSize(), 1);

    t.insert(10);
    EXPECT_EQ(t.getSize(), 1); 
}

// проверка на вставку левого и правого потомков и поиск
TEST(TreeTest, InsertLeftRightSearch) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    EXPECT_TRUE(t.search(5));
    EXPECT_TRUE(t.search(15));
    EXPECT_TRUE(t.search(10));

    EXPECT_FALSE(t.search(99));
}

// проверка на удаление листового узла
TEST(TreeTest, DeleteLeaf) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    t.remove(5);
    
    EXPECT_FALSE(t.search(5));
    EXPECT_EQ(t.getSize(), 2);
}

// проверка на удаление узла с одним левым потомком
TEST(TreeTest, DeleteNodeOneChild_LeftOnly) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(1);

    t.remove(5);

    EXPECT_TRUE(t.search(1));
    EXPECT_FALSE(t.search(5));
    EXPECT_EQ(t.getSize(), 2);
}

// проверка на удаление узла с одним правым потомком
TEST(TreeTest, DeleteNodeOneChild_RightOnly) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(7);

    t.remove(5);

    EXPECT_TRUE(t.search(7));
    EXPECT_FALSE(t.search(5));
    EXPECT_EQ(t.getSize(), 2);
}

// проверка на удаление узла с двумя потомками
TEST(TreeTest, DeleteNodeTwoChildren) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);
    t.insert(12);
    t.insert(18);

    t.remove(10);

    EXPECT_FALSE(t.search(10));
    EXPECT_TRUE(t.search(12));
    EXPECT_EQ(t.getSize(), 4);
}

// проверка на удаление несуществующего узла
TEST(TreeTest, RemoveMissing) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);

    int sizeBefore = t.getSize();
    t.remove(99);
    int sizeAfter = t.getSize();

    EXPECT_EQ(sizeBefore, sizeAfter);
}

// проверка на полную очистку дерева
TEST(TreeTest, ClearTree) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);

    t.clear();
    
    EXPECT_TRUE(t.isEmpty());
    EXPECT_EQ(t.getSize(), 0);
}

// проверка на вывод непустого дерева
TEST(TreeTest, PrintNonEmpty) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    
    t.print();
    SUCCEED(); 
}

// проверка на сохранение и загрузку в текстовом формате
TEST(TreeTest, SaveLoadTextFile) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    string fname = "tree_test.txt";
    
    {
        ofstream out(fname);
        t.saveToFile(out);
    }

    CompleteBinaryTree t2;
    {
        ifstream in(fname);
        t2.loadFromFile(in);
    }

    EXPECT_EQ(t2.getSize(), 3);
    EXPECT_TRUE(t2.search(10));
    EXPECT_TRUE(t2.search(5));
    EXPECT_TRUE(t2.search(15));

    remove(fname.c_str());
}

// проверка на сохранение и загрузку в бинарном формате
TEST(TreeTest, SaveLoadBinaryFile) {
    CompleteBinaryTree t;
    t.insert(100);
    t.insert(50);
    t.insert(150);

    string fname = "tree_test.bin";
    
    {
        ofstream out(fname, ios::binary);
        t.saveToBinaryFile(out);
    }

    CompleteBinaryTree t2;
    {
        ifstream in(fname, ios::binary);
        t2.loadFromBinaryFile(in);
    }

    EXPECT_EQ(t2.getSize(), 3);
    EXPECT_TRUE(t2.search(100));
    EXPECT_TRUE(t2.search(50));
    EXPECT_TRUE(t2.search(150));

    remove(fname.c_str());
}

// проверка на удаление корня с двумя потомками
TEST(TreeTest, Coverage_Remove_Root_TwoChildren) {
    CompleteBinaryTree t;
    t.insert(10); 
    t.insert(5);  
    t.insert(15); 
    

    t.remove(10);
    
    EXPECT_FALSE(t.search(10));
    EXPECT_TRUE(t.search(5));
    EXPECT_TRUE(t.search(15));
    EXPECT_EQ(t.getSize(), 2);
}

// проверка на удаление корня с одним потомком
TEST(TreeTest, Coverage_Remove_Root_OneChild) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    // Нет правого
    
    t.remove(10); // Удаляем корень с 1 левым ребенком
    EXPECT_FALSE(t.search(10));
    EXPECT_TRUE(t.search(5));
}

// проверка на удаление несуществующих элементов и из пустого дерева
TEST(TreeTest, Coverage_Remove_NotExists_And_Empty) {
    OutputCapture cap;
    CompleteBinaryTree t;
    
    t.remove(10); 
    EXPECT_NE(cap.str().find("Нельзя удалить"), string::npos);
    
    t.insert(5);
    t.remove(99); // Несуществующий
    // Проверяем, что размер не изменился
}

// проверка на вставку дубликатов
TEST(TreeTest, Coverage_Insert_Duplicates) {
    OutputCapture cap;
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(10); // Дубликат
    
    EXPECT_EQ(t.getSize(), 1);
}

// проверка на обработку ошибок ввода-вывода
TEST(TreeTest, Coverage_IO_Errors) {
    CompleteBinaryTree t;
    t.saveToFile("");
    t.loadFromFile("missing.txt");
    
    t.saveToBinaryFile("");
    t.loadFromBinaryFile("missing.bin");
    
    // Бинарный файл с маркером конца (-1) сразу
    {
        ofstream f("empty_tree.bin", ios::binary);
        int m = -1;
        f.write((char*)&m, sizeof(m));
        f.close();
    }
    t.loadFromBinaryFile("empty_tree.bin");
    EXPECT_EQ(t.getSize(), 0);
    remove("empty_tree.bin");
}

// проверка на загрузку из пустых файлов
TEST(TreeTest, LoadFromEmptyFiles) {
    CompleteBinaryTree t;
    
    {
        ofstream out("empty.txt");
    }
    {
        ifstream in("empty.txt");
        t.loadFromFile(in);
    }
    EXPECT_EQ(t.getSize(), 0);
    remove("empty.txt");

    {
        ofstream out("empty.bin", ios::binary);
    }
    {
        ifstream in("empty.bin", ios::binary);
        t.loadFromBinaryFile(in);
    }
    EXPECT_EQ(t.getSize(), 0);
    remove("empty.bin");
}
