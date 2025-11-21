#define BOOST_TEST_MODULE StackTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include "../../../zad1/cpp/stack.h"

using namespace std;

string generateRandomString(size_t length = 10) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    static mt19937 rng(random_device{}());
    uniform_int_distribution<size_t> dist(0, sizeof(alphanum) - 2);
    
    string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += alphanum[dist(rng)];
    }
    return result;
}

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(TestConstructorDestructor) {
    Stack stack1;
    BOOST_CHECK_NO_THROW(stack1.print());
    
    Stack* stack2 = new Stack();
    BOOST_CHECK_NO_THROW(delete stack2);
}

BOOST_AUTO_TEST_CASE(TestPushPop) {
    Stack stack;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    BOOST_CHECK_NO_THROW(stack.push(str1));
    BOOST_CHECK_NO_THROW(stack.push(str2));
    BOOST_CHECK_NO_THROW(stack.push(str3));
    
    BOOST_CHECK_NO_THROW(stack.print());
    
    BOOST_CHECK_NO_THROW(stack.pop());
    BOOST_CHECK_NO_THROW(stack.pop());
    BOOST_CHECK_NO_THROW(stack.pop());
    
    BOOST_CHECK_THROW(stack.pop(), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestPrintEmpty) {
    Stack stack;
    BOOST_CHECK_NO_THROW(stack.print());
}

BOOST_AUTO_TEST_CASE(TestFileOperations) {
    const string filename = "test_stack.txt";
    Stack stack1, stack2;
    
    vector<string> testData;
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testData.push_back(str);
        stack1.push(str);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    stack1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    stack2.loadFromFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(stack1.print());
    BOOST_CHECK_NO_THROW(stack2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestBinaryFileOperations) {
    const string filename = "test_stack_binary.bin";
    Stack stack1, stack2;
    
    vector<string> testData;
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testData.push_back(str);
        stack1.push(str);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    stack1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    stack2.loadFromBinaryFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(stack1.print());
    BOOST_CHECK_NO_THROW(stack2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestExceptionSafety) {
    Stack stack;
    
    BOOST_CHECK_THROW(stack.pop(), runtime_error);
    
    string testStr = generateRandomString();
    stack.push(testStr);
    BOOST_CHECK_NO_THROW(stack.pop());
    BOOST_CHECK_THROW(stack.pop(), runtime_error);
}


BOOST_AUTO_TEST_SUITE_END()