#include "gtest/gtest.h"
#include "../stack.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class OutputCapture {
    std::stringstream buffer;
    std::streambuf* prev;
public:
    OutputCapture() : prev(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(prev); }
    std::string str() { return buffer.str(); }
};

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

TEST(StackTest, Branch_Killer_And_Extras) {
    Stack s;
    // Операции на пустом стеке
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

TEST(StackTest, SaveAndLoadFile) {
    OutputCapture cap;
    Stack s;
    
    s.push("hello");
    s.push("world");
    s.saveToFile("stack_test.dat");
    
    Stack s2;
    s2.loadFromFile("stack_test.dat");
    EXPECT_EQ(s2.pop(), "world");
    EXPECT_EQ(s2.pop(), "hello");
    
    remove("stack_test.dat");
}

TEST(StackTest, LoadFromBadFile) {
    OutputCapture cap;
    Stack s;
    s.loadFromFile("nonexistent_file.dat");
    EXPECT_TRUE(s.isEmpty());
    
    { ofstream f("bad_stack.dat", ios::binary); f << "x"; }
    s.loadFromFile("bad_stack.dat");
    remove("bad_stack.dat");
}

TEST(DestructorTest, StackCleanup) {
    OutputCapture cap;
    {
        Stack s;
        s.push("1");
        s.push("2");
    }
}