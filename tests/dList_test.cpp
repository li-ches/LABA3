#include "gtest/gtest.h"
#include "../dList.h"
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

TEST(DListTest, Branch_AllCases) {
    DList dl;
    OutputCapture cap;

    dl.delHead();
    dl.delTail();
    dl.readForward();
    dl.readBackward();
    dl.addBefore("A", "B");
    dl.addAfter("A", "B");

    dl.addHead("A");
    dl.delHead(); 
    EXPECT_EQ(dl.getHead_Test(), nullptr);

    dl.addTail("B");
    dl.delTail();
    EXPECT_EQ(dl.getTail_Test(), nullptr);

    dl.addHead("2");
    dl.addHead("1");
    dl.addTail("3"); 
    
    dl.addAfter("1", "1.5"); 
    dl.addBefore("3", "2.5");
    
    dl.delByVal("1"); // Head
    dl.delByVal("3"); // Tail
    dl.delByVal("2"); // Middle
    
    dl.delAfterValue("NotExists");
    dl.delBeforeValue("NotExists");
    
    dl.addTail("X"); dl.addTail("Y");
    dl.delAfterValue("X"); 
    dl.delBeforeValue("X"); 
    
    EXPECT_TRUE(dl.contains("X"));
}

TEST(DListTest, BranchBooster_Conditions) {
    OutputCapture cap;
    DList d;

    // addHead/Tail logic
    d.addHead("A"); 
    d.addHead("B"); 
    DList d2;
    d2.addTail("A"); 
    d2.addTail("B"); 

    // delHead/Tail logic
    d2.delHead(); 
    d2.delHead(); // Tail becomes null

    DList d3;
    d3.addTail("A");
    d3.delTail(); // Head becomes null

    // delAfter/Before edge
    DList d4;
    d4.addTail("A");
    d4.delAfterValue("Z"); 
    d4.delAfterValue("A"); 
    d4.delBeforeValue("Z");
    d4.delBeforeValue("A");
}

TEST(DListTest, Branch_InsertSpecifics) {
    DList d;
    d.addTail("A"); d.addTail("B"); d.addTail("C"); 
    
    // Middle insertions
    d.addAfter("B", "B_new"); 
    EXPECT_TRUE(d.contains("B_new"));

    // End insertion (addAfter)
    d.addAfter("C", "End");
    EXPECT_EQ(d.getTail_Test()->value, "End");

    // Middle insertion (addBefore)
    d.addBefore("B", "A_new");
    EXPECT_TRUE(d.contains("A_new"));

    // Start insertion (addBefore)
    d.addBefore("A", "Start");
    EXPECT_EQ(d.getHead_Test()->value, "Start");
}

TEST(DListTest, Branch_Killer) {
    DList d;
    d.delAfterValue("Missing"); 
    d.addTail("A");
    d.delAfterValue("A"); 
    d.delBeforeValue("Missing");
    d.delBeforeValue("A");

    DList empty;
    empty.delHead(); 
    empty.delTail(); 
    empty.readForward(); 
    empty.readBackward(); 
    empty.addBefore("Target", "Val"); 
    empty.addAfter("Target", "Val");
}

TEST(DListTest, SaveAndLoadFile) {
    OutputCapture cap;
    DList d;
    d.addTail("first");
    d.addTail("second");
    d.saveToFile("dlist_test.dat");
    
    DList d2;
    d2.loadFromFile("dlist_test.dat");
    EXPECT_TRUE(d2.contains("first"));
    EXPECT_TRUE(d2.contains("second"));
    remove("dlist_test.dat");
}