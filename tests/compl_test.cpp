#include "gtest/gtest.h"
#include "../compl.h"
#include <sstream>
#include <iostream>

class OutputCapture {
    std::stringstream buffer;
    std::streambuf* old;
public:
    OutputCapture() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(old); }
    std::string str() const { return buffer.str(); }
};

TEST(TreeTest, EmptySearchPrintRemove) {
    CompleteBinaryTree t;

    {   // search empty
        OutputCapture cap;
        EXPECT_FALSE(t.search(10));
        EXPECT_NE(cap.str().find("пустом"), std::string::npos);
    }

    {   // remove empty
        OutputCapture cap;
        t.remove(5);
        EXPECT_NE(cap.str().find("пуст"), std::string::npos);
    }

    {   // print empty
        OutputCapture cap;
        t.print();
        EXPECT_NE(cap.str().find("пуст"), std::string::npos);
    }
}


TEST(TreeTest, InsertRootAndDuplicate) {
    CompleteBinaryTree t;

    {   // insert root
        OutputCapture cap;
        t.insert(10);
        EXPECT_NE(cap.str().find("добавлен"), std::string::npos);
    }

    {   // duplicate
        OutputCapture cap;
        t.insert(10);
        EXPECT_NE(cap.str().find("существует"), std::string::npos);
    }
}

TEST(TreeTest, InsertLeftRightSearch) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    {   // search existing
        OutputCapture cap;
        EXPECT_TRUE(t.search(5));
        EXPECT_NE(cap.str().find("найден"), std::string::npos);
    }

    {   // search missing
        OutputCapture cap;
        EXPECT_FALSE(t.search(99));
        EXPECT_NE(cap.str().find("не найден"), std::string::npos);
    }
}

TEST(TreeTest, DeleteLeaf) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    {   // delete leaf
        OutputCapture cap;
        t.remove(5);
        EXPECT_NE(cap.str().find("удален"), std::string::npos);
    }

    EXPECT_FALSE(t.search(5));
}

TEST(TreeTest, DeleteNodeOneChild) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(1);    // 5 has one left child

    {  
        OutputCapture cap;
        t.remove(5);
        EXPECT_NE(cap.str().find("удален"), std::string::npos);
    }

    EXPECT_TRUE(t.search(1));
    EXPECT_FALSE(t.search(5));
}

TEST(TreeTest, DeleteNodeTwoChildren) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);
    t.insert(12);
    t.insert(18);

    {  
        OutputCapture cap;
        t.remove(10);    // successor = 12
        EXPECT_NE(cap.str().find("удален"), std::string::npos);
    }

    EXPECT_FALSE(t.search(10));
    EXPECT_TRUE(t.search(12));
}

TEST(TreeTest, RemoveMissing) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);

    {  
        OutputCapture cap;
        t.remove(99);
        EXPECT_NE(cap.str().find("не найден"), std::string::npos);
    }
}

TEST(TreeTest, ClearTree) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);

    {  
        OutputCapture cap;
        t.clear();
        EXPECT_NE(cap.str().find("очищено"), std::string::npos);
    }

    EXPECT_TRUE(t.isEmpty());
}

TEST(TreeTest, PrintNonEmpty) {
    CompleteBinaryTree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    OutputCapture cap;
    t.print();

    EXPECT_NE(cap.str().find("10"), std::string::npos);
    EXPECT_NE(cap.str().find("5"), std::string::npos);
    EXPECT_NE(cap.str().find("15"), std::string::npos);
}
