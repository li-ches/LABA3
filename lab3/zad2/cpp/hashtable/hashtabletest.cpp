#define BOOST_TEST_MODULE HashTableTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include <vector>
#include "../../../zad1/cpp/hash-table.h"

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

vector<pair<string, string>> generateRandomKeyValuePairs(int count) {
    vector<pair<string, string>> pairs;
    for (int i = 0; i < count; ++i) {
        pairs.emplace_back(generateRandomString(8), generateRandomString(15));
    }
    return pairs;
}

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(TestCuckooConstructorDestructor) {
    CuckooHashTable table1;
    BOOST_CHECK_NO_THROW(table1.print());
    
    CuckooHashTable* table2 = new CuckooHashTable(20);
    BOOST_CHECK_NO_THROW(table2->print());
    BOOST_CHECK_NO_THROW(delete table2);
}

BOOST_AUTO_TEST_CASE(TestDoubleConstructorDestructor) {
    DoubleHashTable table1;
    BOOST_CHECK_NO_THROW(table1.print());
    
    DoubleHashTable* table2 = new DoubleHashTable(20);
    BOOST_CHECK_NO_THROW(table2->print());
    BOOST_CHECK_NO_THROW(delete table2);
}

BOOST_AUTO_TEST_CASE(TestCuckooInsertSearch) {
    CuckooHashTable table;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestDoubleInsertSearch) {
    DoubleHashTable table;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestCuckooInsertDuplicate) {
    CuckooHashTable table;
    string key = generateRandomString();
    string value1 = generateRandomString();
    string value2 = generateRandomString();
    
    BOOST_CHECK(table.insert(key, value1));
    BOOST_CHECK_EQUAL(table.search(key), value1);
    
    BOOST_CHECK(table.insert(key, value2));
    BOOST_CHECK_EQUAL(table.search(key), value2);
}

BOOST_AUTO_TEST_CASE(TestDoubleInsertDuplicate) {
    DoubleHashTable table;
    string key = generateRandomString();
    string value1 = generateRandomString();
    string value2 = generateRandomString();
    
    BOOST_CHECK(table.insert(key, value1));
    BOOST_CHECK_EQUAL(table.search(key), value1);
    
    BOOST_CHECK(table.insert(key, value2));
    BOOST_CHECK_EQUAL(table.search(key), value2);
}

BOOST_AUTO_TEST_CASE(TestCuckooRemove) {
    CuckooHashTable table;
    auto pairs = generateRandomKeyValuePairs(3);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table.insert(pair.first, data);
    }
    
    BOOST_CHECK(table.remove(pairs[0].first));
    BOOST_CHECK_EQUAL(table.search(pairs[0].first), "");
    
    BOOST_CHECK(table.remove(pairs[1].first));
    BOOST_CHECK_EQUAL(table.search(pairs[1].first), "");
    
    BOOST_CHECK_EQUAL(table.search(pairs[2].first), pairs[2].second);
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestDoubleRemove) {
    DoubleHashTable table;
    auto pairs = generateRandomKeyValuePairs(3);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table.insert(pair.first, data);
    }
    
    BOOST_CHECK(table.remove(pairs[0].first));
    BOOST_CHECK_EQUAL(table.search(pairs[0].first), "");
    
    BOOST_CHECK(table.remove(pairs[1].first));
    BOOST_CHECK_EQUAL(table.search(pairs[1].first), "");
    
    BOOST_CHECK_EQUAL(table.search(pairs[2].first), pairs[2].second);
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestCuckooRehash) {
    CuckooHashTable table(5);
    auto pairs = generateRandomKeyValuePairs(10);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
    }
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestDoubleRehash) {
    DoubleHashTable table(5);
    auto pairs = generateRandomKeyValuePairs(10);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
    }
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    BOOST_CHECK_NO_THROW(table.print());
}

BOOST_AUTO_TEST_CASE(TestCuckooGetAllElements) {
    CuckooHashTable table;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table.insert(pair.first, data);
    }
    
    auto elements = table.getAllElements();
    BOOST_CHECK_EQUAL(elements.size(), pairs.size());
    
    for (const auto& pair : pairs) {
        bool found = false;
        for (const auto& elem : elements) {
            if (elem.first == pair.first && elem.second == pair.second) {
                found = true;
                break;
            }
        }
        BOOST_CHECK(found);
    }
}

BOOST_AUTO_TEST_CASE(TestDoubleGetAllElements) {
    DoubleHashTable table;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table.insert(pair.first, data);
    }
    
    auto elements = table.getAllElements();
    BOOST_CHECK_EQUAL(elements.size(), pairs.size());
    
    for (const auto& pair : pairs) {
        bool found = false;
        for (const auto& elem : elements) {
            if (elem.first == pair.first && elem.second == pair.second) {
                found = true;
                break;
            }
        }
        BOOST_CHECK(found);
    }
}

BOOST_AUTO_TEST_CASE(TestCuckooFileOperations) {
    const string filename = "test_cuckoo.txt";
    CuckooHashTable table1, table2;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table1.insert(pair.first, data);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    table1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    table2.loadFromFile(inFile);
    inFile.close();
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table2.search(pair.first), pair.second);
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestDoubleFileOperations) {
    const string filename = "test_double.txt";
    DoubleHashTable table1, table2;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table1.insert(pair.first, data);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    table1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    table2.loadFromFile(inFile);
    inFile.close();
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table2.search(pair.first), pair.second);
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestCuckooBinaryFileOperations) {
    const string filename = "test_cuckoo_binary.bin";
    CuckooHashTable table1, table2;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table1.insert(pair.first, data);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    table1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    table2.loadFromBinaryFile(inFile);
    inFile.close();
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table2.search(pair.first), pair.second);
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestDoubleBinaryFileOperations) {
    const string filename = "test_double_binary.bin";
    DoubleHashTable table1, table2;
    auto pairs = generateRandomKeyValuePairs(5);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        table1.insert(pair.first, data);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    table1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    table2.loadFromBinaryFile(inFile);
    inFile.close();
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table2.search(pair.first), pair.second);
    }
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestCuckooLargeData) {
    CuckooHashTable table;
    auto pairs = generateRandomKeyValuePairs(50);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
    }
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    for (size_t i = 0; i < pairs.size() / 2; ++i) {
        BOOST_CHECK(table.remove(pairs[i].first));
    }
    
    for (size_t i = 0; i < pairs.size() / 2; ++i) {
        BOOST_CHECK_EQUAL(table.search(pairs[i].first), "");
    }
    
    for (size_t i = pairs.size() / 2; i < pairs.size(); ++i) {
        BOOST_CHECK_EQUAL(table.search(pairs[i].first), pairs[i].second);
    }
}

BOOST_AUTO_TEST_CASE(TestDoubleLargeData) {
    DoubleHashTable table;
    auto pairs = generateRandomKeyValuePairs(50);
    
    for (const auto& pair : pairs) {
        string data = pair.second;
        BOOST_CHECK(table.insert(pair.first, data));
    }
    
    for (const auto& pair : pairs) {
        BOOST_CHECK_EQUAL(table.search(pair.first), pair.second);
    }
    
    for (size_t i = 0; i < pairs.size() / 2; ++i) {
        BOOST_CHECK(table.remove(pairs[i].first));
    }
    
    for (size_t i = 0; i < pairs.size() / 2; ++i) {
        BOOST_CHECK_EQUAL(table.search(pairs[i].first), "");
    }
    
    for (size_t i = pairs.size() / 2; i < pairs.size(); ++i) {
        BOOST_CHECK_EQUAL(table.search(pairs[i].first), pairs[i].second);
    }
}

BOOST_AUTO_TEST_CASE(TestCuckooEmptyKey) {
    CuckooHashTable table;
    string value = "value";
    
    BOOST_CHECK(!table.insert("", value));
    BOOST_CHECK_EQUAL(table.search(""), "");
    BOOST_CHECK(!table.remove(""));
}

BOOST_AUTO_TEST_CASE(TestDoubleEmptyKey) {
    DoubleHashTable table;
    string value = "value";
    
    BOOST_CHECK(!table.insert("", value));
    BOOST_CHECK_EQUAL(table.search(""), "");
    BOOST_CHECK(!table.remove(""));
}

BOOST_AUTO_TEST_SUITE_END()