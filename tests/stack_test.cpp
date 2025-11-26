#include "gtest/gtest.h"
#include "../stack.h"
#include "../stack_serialize.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class OutputCapture {
    stringstream buffer;
    streambuf* prev;
public:
    OutputCapture() : prev(cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { cout.rdbuf(prev); }
    string str() { return buffer.str(); }
};

// проверка на основные операции стека и граничные случаи
TEST(StackTest, Branch_Coverage) {
    Stack s;
    OutputCapture cap;

    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    s.readStack();

    s.push("A");
    EXPECT_EQ(s.peek(), "A");
    EXPECT_EQ(s.pop(), "A");
}

// проверка на расширенные сценарии работы стека (LIFO)
TEST(StackTest, Branch_Killer_And_Extras) {
    Stack s;
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    
    OutputCapture cap; 
    s.readStack();

    s.push("one"); s.push("two"); s.push("three");
    EXPECT_EQ(s.pop(), "three");
    EXPECT_EQ(s.pop(), "two");
    EXPECT_EQ(s.pop(), "one");
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
}

// проверка на сохранение и загрузку стека из файла
TEST(StackTest, SaveAndLoadFile) {
    OutputCapture cap;
    Stack s;
    
    s.push("hello");
    s.push("world");
    StackSerializer::saveToFile(s, "stack_test.dat");
    
    Stack s2;
    StackSerializer::loadFromFile(s2, "stack_test.dat");
    EXPECT_EQ(s2.pop(), "world");
    EXPECT_EQ(s2.pop(), "hello");
    
    remove("stack_test.dat");
}

// проверка на загрузку из несуществующих и поврежденных файлов
TEST(StackTest, LoadFromBadFile) {
    OutputCapture cap;
    Stack s;
    StackSerializer::loadFromFile(s, "nonexistent_file.dat");
    EXPECT_TRUE(s.isEmpty());
    
    { ofstream f("bad_stack.dat", ios::binary); f << "x"; }
    StackSerializer::loadFromFile(s, "bad_stack.dat");
    remove("bad_stack.dat");
}

// проверка на корректность работы деструктора стека
TEST(DestructorTest, StackCleanup) {
    OutputCapture cap;
    {
        Stack s;
        s.push("1");
        s.push("2");
    }
}

// проверка на операции с пустым стеком
TEST(StackTest, Coverage_EmptyOps) {
    Stack s;
    OutputCapture cap;
    
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    
    s.readStack();
    EXPECT_NE(cap.str().find("пуст"), string::npos);
}

// проверка на обработку ошибок ввода-вывода
TEST(StackTest, Coverage_IO) {
    Stack s;
    StackSerializer::saveToFile(s, "");
    StackSerializer::loadFromFile(s, "missing.dat");
    
    {
        ofstream f("bad_stack.bin", ios::binary);
        int c = 10;
        f.write((char*)&c, sizeof(c));
        f.close();
    }
    StackSerializer::loadFromBinaryFile(s, "bad_stack.bin");
    remove("bad_stack.bin");
}