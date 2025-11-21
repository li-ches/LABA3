#define BOOST_TEST_MODULE QueueTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include "../../../zad1/cpp/queue.h"

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

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(TestConstructorDestructor) {
    Queue queue1;
    BOOST_CHECK_NO_THROW(queue1.print());
    
    Queue* queue2 = new Queue();
    BOOST_CHECK_NO_THROW(queue2->print());
    BOOST_CHECK_NO_THROW(delete queue2);
}

BOOST_AUTO_TEST_CASE(TestEnqueueDequeue) {
    Queue queue;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    BOOST_CHECK_NO_THROW(queue.enqueue(str1));
    BOOST_CHECK_NO_THROW(queue.enqueue(str2));
    BOOST_CHECK_NO_THROW(queue.enqueue(str3));
    BOOST_CHECK_NO_THROW(queue.print());
    
    BOOST_CHECK_NO_THROW(queue.dequeue());
    BOOST_CHECK_NO_THROW(queue.dequeue());
    BOOST_CHECK_NO_THROW(queue.dequeue());
    BOOST_CHECK_THROW(queue.dequeue(), runtime_error);
}


BOOST_AUTO_TEST_CASE(TestPrintEmpty) {
    Queue queue;
    BOOST_CHECK_NO_THROW(queue.print());
}

BOOST_AUTO_TEST_CASE(TestFileOperations) {
    const string filename = "test_queue.txt";
    Queue queue1, queue2;
    
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        queue1.enqueue(str);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    queue1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    queue2.loadFromFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(queue1.print());
    BOOST_CHECK_NO_THROW(queue2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestBinaryFileOperations) {
    const string filename = "test_queue_binary.bin";
    Queue queue1, queue2;
    
    vector<string> testStrings;
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testStrings.push_back(str);
        queue1.enqueue(str);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    queue1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    queue2.loadFromBinaryFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(queue1.print());
    BOOST_CHECK_NO_THROW(queue2.print());
    
    for (const auto& str : testStrings) {
        BOOST_CHECK_NO_THROW(queue2.dequeue());
    }
    
    BOOST_CHECK_THROW(queue2.dequeue(), runtime_error);
    
    remove(filename.c_str());
}


BOOST_AUTO_TEST_SUITE_END()