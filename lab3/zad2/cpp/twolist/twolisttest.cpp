#define BOOST_TEST_MODULE TwoListTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include "../../../zad1/cpp/twolist.h"

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

BOOST_AUTO_TEST_SUITE(TwoListTests)

BOOST_AUTO_TEST_CASE(TestConstructorDestructor) {
    TwoList list1;
    BOOST_CHECK_NO_THROW(list1.print());
    
    TwoList* list2 = new TwoList();
    BOOST_CHECK_NO_THROW(delete list2);
}

BOOST_AUTO_TEST_CASE(TestAddToHead) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    BOOST_CHECK_NO_THROW(list.addToHead(str1));
    BOOST_CHECK_NO_THROW(list.addToHead(str2));
    BOOST_CHECK_NO_THROW(list.addToHead(str3));
    
    BOOST_CHECK_NO_THROW(list.print());
}

BOOST_AUTO_TEST_CASE(TestAddToTail) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    BOOST_CHECK_NO_THROW(list.addToTail(str1));
    BOOST_CHECK_NO_THROW(list.addToTail(str2));
    BOOST_CHECK_NO_THROW(list.addToTail(str3));
    
    BOOST_CHECK_NO_THROW(list.print());
}

BOOST_AUTO_TEST_CASE(TestAddAfterValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string str4 = generateRandomString();
    string str5 = generateRandomString();
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    
    BOOST_CHECK_NO_THROW(list.addAfterValue(str2, str4));
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_NO_THROW(list.addAfterValue(str3, str5));
    BOOST_CHECK_NO_THROW(list.print());
}    

BOOST_AUTO_TEST_CASE(TestAddBeforeValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string str4 = generateRandomString();
    string str5 = generateRandomString();
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    
    BOOST_CHECK_NO_THROW(list.addBeforeValue(str2, str4));
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_NO_THROW(list.addBeforeValue(str1, str5));
    BOOST_CHECK_NO_THROW(list.print());
    
}

BOOST_AUTO_TEST_CASE(TestRemoveFromHead) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    
    BOOST_CHECK_NO_THROW(list.removeFromHead());
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_NO_THROW(list.removeFromHead());
    BOOST_CHECK_THROW(list.removeFromHead(), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveFromTail) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    
    BOOST_CHECK_NO_THROW(list.removeFromTail());
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_NO_THROW(list.removeFromTail());
    BOOST_CHECK_THROW(list.removeFromTail(), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveAfterValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    
    BOOST_CHECK_NO_THROW(list.removeAfterValue(str1));
    BOOST_CHECK_NO_THROW(list.print());

    BOOST_CHECK_THROW(list.removeAfterValue(str3), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveBeforeValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string str4 = generateRandomString();
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    list.addToTail(str4);
    BOOST_CHECK_NO_THROW(list.removeBeforeValue(str2));
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_NO_THROW(list.removeBeforeValue(str4));
    BOOST_CHECK_NO_THROW(list.print());
    BOOST_CHECK_THROW(list.removeBeforeValue(str1), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveByValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    
    BOOST_CHECK_NO_THROW(list.removeByValue(str2));
    BOOST_CHECK_NO_THROW(list.print());
    
    BOOST_CHECK_NO_THROW(list.removeByValue(str1));
    BOOST_CHECK_NO_THROW(list.print());
    
    BOOST_CHECK_NO_THROW(list.removeByValue(str3));
    BOOST_CHECK_NO_THROW(list.print());
}

BOOST_AUTO_TEST_CASE(TestGetElementByValue) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    list.addToTail(str3);
    
    BOOST_CHECK_EQUAL(list.getElementByValue(str2), str2);
    BOOST_CHECK_EQUAL(list.getElementByValue(str1), str1);
    BOOST_CHECK_EQUAL(list.getElementByValue(str3), str3);
    
}

BOOST_AUTO_TEST_CASE(TestFindByVal) {
    TwoList list;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    
    list.addToTail(str1);
    list.addToTail(str2);
    
    BOOST_CHECK(list.getElementByValue(str1) == str1);
    BOOST_CHECK(list.getElementByValue(str2) == str2);
}

BOOST_AUTO_TEST_CASE(TestPrintEmpty) {
    TwoList list;
    BOOST_CHECK_NO_THROW(list.print());
}

BOOST_AUTO_TEST_CASE(TestFileOperations) {
    const string filename = "test_twolist.txt";
    TwoList list1, list2;
    
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        list1.addToTail(str);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    list1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    list2.loadFromFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(list1.print());
    BOOST_CHECK_NO_THROW(list2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestBinaryFileOperations) {
    const string filename = "test_twolist_binary.bin";
    TwoList list1, list2;
    
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        list1.addToTail(str);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    list1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    list2.loadFromBinaryFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(list1.print());
    BOOST_CHECK_NO_THROW(list2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_SUITE_END()