#define BOOST_TEST_MODULE DynArrTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include "../../../zad1/cpp/dynarr.h"

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

BOOST_AUTO_TEST_SUITE(DynArrTests)

BOOST_AUTO_TEST_CASE(TestConstructorDestructor) {
    DynArr arr1;
    BOOST_CHECK_EQUAL(arr1.getLength(), 0);
    
    DynArr arr2(20);
    BOOST_CHECK_EQUAL(arr2.getLength(), 0);
    
    DynArr* arr3 = new DynArr();
    BOOST_CHECK_NO_THROW(delete arr3);
}

BOOST_AUTO_TEST_CASE(TestAddToEnd) {
    DynArr arr(2);
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    
    arr.addToEnd(str1);
    BOOST_CHECK_EQUAL(arr.getLength(), 1);
    BOOST_CHECK_EQUAL(arr.getElement(0), str1);
    
    arr.addToEnd(str2);
    BOOST_CHECK_EQUAL(arr.getLength(), 2);
    BOOST_CHECK_EQUAL(arr.getElement(1), str2);
    
    arr.addToEnd(str3);
    BOOST_CHECK_EQUAL(arr.getLength(), 3);
    BOOST_CHECK_EQUAL(arr.getElement(2), str3);
    
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        arr.addToEnd(str);
        BOOST_CHECK_EQUAL(arr.getElement(3 + i), str);
    }
}

BOOST_AUTO_TEST_CASE(TestAddAtIndex) {
    DynArr arr(3);
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string str4 = generateRandomString();
    string str5 = generateRandomString();
    
    arr.addAtIndex(0, str1);
    BOOST_CHECK_EQUAL(arr.getLength(), 1);
    BOOST_CHECK_EQUAL(arr.getElement(0), str1);
    
    arr.addAtIndex(0, str2);
    BOOST_CHECK_EQUAL(arr.getLength(), 2);
    BOOST_CHECK_EQUAL(arr.getElement(0), str2);
    BOOST_CHECK_EQUAL(arr.getElement(1), str1);
    
    arr.addAtIndex(2, str3);
    BOOST_CHECK_EQUAL(arr.getLength(), 3);
    BOOST_CHECK_EQUAL(arr.getElement(2), str3);
    
    arr.addAtIndex(1, str4);
    BOOST_CHECK_EQUAL(arr.getLength(), 4);
    BOOST_CHECK_EQUAL(arr.getElement(1), str4);
    
    arr.addAtIndex(2, str5);
    BOOST_CHECK_EQUAL(arr.getLength(), 5);
    
    BOOST_CHECK_THROW(arr.addAtIndex(-1, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.addAtIndex(10, generateRandomString()), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestGetElement) {
    DynArr arr;
    vector<string> testStrings;
    
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testStrings.push_back(str);
        arr.addToEnd(str);
    }
    
    for (int i = 0; i < 5; ++i) {
        BOOST_CHECK_EQUAL(arr.getElement(i), testStrings[i]);
    }
    
    BOOST_CHECK_THROW(arr.getElement(-1), runtime_error);
    BOOST_CHECK_THROW(arr.getElement(5), runtime_error);
    BOOST_CHECK_THROW(arr.getElement(100), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestReplaceElement) {
    DynArr arr;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string new1 = generateRandomString();
    string new2 = generateRandomString();
    string new3 = generateRandomString();
    
    arr.addToEnd(str1);
    arr.addToEnd(str2);
    arr.addToEnd(str3);
    
    arr.replaceElement(0, new1);
    BOOST_CHECK_EQUAL(arr.getElement(0), new1);
    
    arr.replaceElement(1, new2);
    BOOST_CHECK_EQUAL(arr.getElement(1), new2);
    
    arr.replaceElement(2, new3);
    BOOST_CHECK_EQUAL(arr.getElement(2), new3);
    
    BOOST_CHECK_THROW(arr.replaceElement(-1, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.replaceElement(3, generateRandomString()), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveAtIndex) {
    DynArr arr;
    string str1 = generateRandomString();
    string str2 = generateRandomString();
    string str3 = generateRandomString();
    string str4 = generateRandomString();
    string str5 = generateRandomString();
    
    arr.addToEnd(str1);
    arr.addToEnd(str2);
    arr.addToEnd(str3);
    arr.addToEnd(str4);
    arr.addToEnd(str5);
    
    arr.removeAtIndex(2);
    BOOST_CHECK_EQUAL(arr.getLength(), 4);
    BOOST_CHECK_EQUAL(arr.getElement(0), str1);
    BOOST_CHECK_EQUAL(arr.getElement(1), str2);
    BOOST_CHECK_EQUAL(arr.getElement(2), str4);
    BOOST_CHECK_EQUAL(arr.getElement(3), str5);
    
    arr.removeAtIndex(3);
    BOOST_CHECK_EQUAL(arr.getLength(), 3);
    BOOST_CHECK_EQUAL(arr.getElement(0), str1);
    BOOST_CHECK_EQUAL(arr.getElement(1), str2);
    BOOST_CHECK_EQUAL(arr.getElement(2), str4);
    
    arr.removeAtIndex(0);
    BOOST_CHECK_EQUAL(arr.getLength(), 2);
    BOOST_CHECK_EQUAL(arr.getElement(0), str2);
    BOOST_CHECK_EQUAL(arr.getElement(1), str4);
    
    arr.removeAtIndex(0);
    BOOST_CHECK_EQUAL(arr.getLength(), 1);
    BOOST_CHECK_EQUAL(arr.getElement(0), str4);
    
    arr.removeAtIndex(0);
    BOOST_CHECK_EQUAL(arr.getLength(), 0);
    
    BOOST_CHECK_THROW(arr.removeAtIndex(-1), runtime_error);
    BOOST_CHECK_THROW(arr.removeAtIndex(0), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestPrintArr) {
    DynArr arr;
    BOOST_CHECK_NO_THROW(arr.printArr());
    
    arr.addToEnd(generateRandomString());
    arr.addToEnd(generateRandomString());
    BOOST_CHECK_NO_THROW(arr.printArr());
}

BOOST_AUTO_TEST_CASE(TestFileOperations) {
    const string filename = "test_dynarr.txt";
    DynArr arr1, arr2;
    
    vector<string> testData;
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testData.push_back(str);
        arr1.addToEnd(str);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    arr1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    arr2.loadFromFile(inFile);
    inFile.close();
    
    BOOST_CHECK_EQUAL(arr1.getLength(), arr2.getLength());
    for (int i = 0; i < arr1.getLength(); ++i) {
        BOOST_CHECK_EQUAL(arr1.getElement(i), arr2.getElement(i));
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestBinaryFileOperations) {
    const string filename = "test_dynarr_binary.bin";
    DynArr arr1, arr2;
    
    vector<string> testData;
    for (int i = 0; i < 5; ++i) {
        string str = generateRandomString();
        testData.push_back(str);
        arr1.addToEnd(str);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    arr1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    arr2.loadFromBinaryFile(inFile);
    inFile.close();
    
    BOOST_CHECK_EQUAL(arr1.getLength(), arr2.getLength());
    for (int i = 0; i < arr1.getLength(); ++i) {
        BOOST_CHECK_EQUAL(arr1.getElement(i), arr2.getElement(i));
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestExceptionSafety) {
    DynArr arr;
    
    BOOST_CHECK_THROW(arr.getElement(0), runtime_error);
    BOOST_CHECK_THROW(arr.replaceElement(0, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.removeAtIndex(0), runtime_error);
    
    string testStr = generateRandomString();
    arr.addToEnd(testStr);
    
    BOOST_CHECK_THROW(arr.getElement(-1), runtime_error);
    BOOST_CHECK_THROW(arr.getElement(1), runtime_error);
    BOOST_CHECK_THROW(arr.replaceElement(-1, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.replaceElement(1, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.removeAtIndex(-1), runtime_error);
    BOOST_CHECK_THROW(arr.removeAtIndex(1), runtime_error);
    
    BOOST_CHECK_THROW(arr.addAtIndex(-1, generateRandomString()), runtime_error);
    BOOST_CHECK_THROW(arr.addAtIndex(2, generateRandomString()), runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()