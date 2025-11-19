#include "gtest/gtest.h"
#include "../queue.h"
#include "../serialize.h" 
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

TEST(QueueTest, Branch_Coverage) {
    Queue q;
    OutputCapture cap;

    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.pop(), "[QUEUE_EMPTY]");
    EXPECT_EQ(q.peek(), "[QUEUE_EMPTY]");
    q.print();

    q.push("A");
    EXPECT_EQ(q.peek(), "A");
    EXPECT_EQ(q.pop(), "A"); 
    
    q.push("1");
    q.push("2");
    EXPECT_EQ(q.pop(), "1");
    EXPECT_EQ(q.pop(), "2");
}

TEST(QueueTest, BranchBooster_PopEmpty) {
    OutputCapture cap;
    Queue q;
    
    q.push("A");
    q.push("B");
    q.pop(); // front -> B
    q.pop(); // front -> null, rear -> null
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, SaveAndLoadFile) {
    OutputCapture cap;
    Queue q;
    
    q.push("first");
    q.push("second");
    q.saveToFile("queue_test.dat");
    
    Queue q2;
    q2.loadFromFile("queue_test.dat");
    EXPECT_EQ(q2.pop(), "first");
    EXPECT_EQ(q2.pop(), "second");
    
    remove("queue_test.dat");
}

TEST(QueueTest, LoadFromBadFile) {
    OutputCapture cap;
    Queue q;
    q.loadFromFile("nonexistent_queue.dat");
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, TruncatedFile) {
    std::ofstream out("q_trunc.dat", std::ios::binary);
    int count = 2;
    out.write(reinterpret_cast<char*>(&count), sizeof(count));
    writeString(out, std::string("only"));
    out.close();

    Queue q; q.loadFromFile("q_trunc.dat");
    remove("q_trunc.dat");
}

TEST(DestructorTest, QueueCleanup) {
    OutputCapture cap;
    {
        Queue q;
        q.push("1");
        q.push("2");
    }
}