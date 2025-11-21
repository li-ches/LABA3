#define BOOST_TEST_MODULE RBTreeTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <random>
#include <algorithm>
#include <vector>
#include "../../../zad1/cpp/rbtree.h"

using namespace std;

int generateRandomNumber(int min = 1, int max = 1000) {
    static mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

vector<int> generateRandomNumbers(int count, int min = 1, int max = 1000) {
    vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        numbers.push_back(generateRandomNumber(min, max));
    }
    return numbers;
}

BOOST_AUTO_TEST_SUITE(RBTreeTests)

BOOST_AUTO_TEST_CASE(TestConstructorDestructor) {
    RBTree tree1;
    BOOST_CHECK_NO_THROW(tree1.print());
    
    RBTree* tree2 = new RBTree();
    BOOST_CHECK_NO_THROW(tree2->print());
    BOOST_CHECK_NO_THROW(delete tree2);
}

BOOST_AUTO_TEST_CASE(TestInsert) {
    RBTree tree;
    vector<int> numbers = generateRandomNumbers(50);
    
    for (int num : numbers) {
        BOOST_CHECK_NO_THROW(tree.insert(num));
    }
    
    BOOST_CHECK_NO_THROW(tree.print());
}

BOOST_AUTO_TEST_CASE(TestInsertDuplicate) {
    RBTree tree;
    int num = generateRandomNumber();
    
    BOOST_CHECK_NO_THROW(tree.insert(num));
    BOOST_CHECK_NO_THROW(tree.insert(num));
    BOOST_CHECK_NO_THROW(tree.insert(num));
    
    BOOST_CHECK_NO_THROW(tree.print());
}

BOOST_AUTO_TEST_CASE(TestFind) {
    RBTree tree;
    vector<int> numbers = generateRandomNumbers(3);
    
    BOOST_CHECK(tree.find(numbers[0]) == tree.nil);
    
    for (int num : numbers) {
        tree.insert(num);
    }
    
    for (int num : numbers) {
        BOOST_CHECK(tree.find(num) != tree.nil);
    }
    
    BOOST_CHECK(tree.find(generateRandomNumber(1001, 2000)) == tree.nil);
    BOOST_CHECK(tree.find(generateRandomNumber(1001, 2000)) == tree.nil);
}

BOOST_AUTO_TEST_CASE(TestRemove) {
    RBTree tree;
    vector<int> numbers = generateRandomNumbers(5);
    
    BOOST_CHECK_THROW(tree.remove(numbers[0]), runtime_error);
    
    for (int num : numbers) {
        tree.insert(num);
    }
    
    BOOST_CHECK_NO_THROW(tree.remove(numbers[2]));
    BOOST_CHECK(tree.find(numbers[2]) == tree.nil);
    
    BOOST_CHECK_NO_THROW(tree.remove(numbers[0]));
    BOOST_CHECK(tree.find(numbers[0]) == tree.nil);
    
    BOOST_CHECK_NO_THROW(tree.remove(numbers[4]));
    BOOST_CHECK(tree.find(numbers[4]) == tree.nil);
    
    BOOST_CHECK_NO_THROW(tree.print());
    
    BOOST_CHECK_THROW(tree.remove(generateRandomNumber(1001, 2000)), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestRemoveRoot) {
    RBTree tree;
    int num = generateRandomNumber();
    
    tree.insert(num);
    BOOST_CHECK_NO_THROW(tree.remove(num));
    BOOST_CHECK(tree.find(num) == tree.nil);
    BOOST_CHECK_NO_THROW(tree.print());
}

BOOST_AUTO_TEST_CASE(TestRemoveAllElements) {
    RBTree tree;
    vector<int> numbers = generateRandomNumbers(150);
    
    for (int num : numbers) {
        tree.insert(num);
    }
    
    for (int num : numbers) {
        BOOST_CHECK_NO_THROW(tree.remove(num));
        BOOST_CHECK(tree.find(num) == tree.nil);
    }
    
    BOOST_CHECK_NO_THROW(tree.print());
    BOOST_CHECK_THROW(tree.remove(numbers[0]), runtime_error);
}

BOOST_AUTO_TEST_CASE(TestFileOperations) {
    const string filename = "test_rbtree.txt";
    RBTree tree1, tree2;
    vector<int> numbers = generateRandomNumbers(7);
    
    for (int num : numbers) {
        tree1.insert(num);
    }
    
    ofstream outFile(filename);
    BOOST_REQUIRE(outFile.is_open());
    tree1.saveToFile(outFile);
    outFile.close();
    
    ifstream inFile(filename);
    BOOST_REQUIRE(inFile.is_open());
    tree2.loadFromFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(tree1.print());
    BOOST_CHECK_NO_THROW(tree2.print());
    
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestBinaryFileOperations) {
    const string filename = "test_rbtree_binary.bin";
    RBTree tree1, tree2;
    vector<int> numbers = generateRandomNumbers(7);
    
    for (int num : numbers) {
        tree1.insert(num);
    }
    
    ofstream outFile(filename, ios::binary);
    BOOST_REQUIRE(outFile.is_open());
    tree1.saveToBinaryFile(outFile);
    outFile.close();
    
    ifstream inFile(filename, ios::binary);
    BOOST_REQUIRE(inFile.is_open());
    tree2.loadFromBinaryFile(inFile);
    inFile.close();
    
    BOOST_CHECK_NO_THROW(tree1.print());
    BOOST_CHECK_NO_THROW(tree2.print());
    remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(TestLargeTree) {
    RBTree tree;
    vector<int> numbers = generateRandomNumbers(100, 1, 1000);

    sort(numbers.begin(), numbers.end());
    numbers.erase(unique(numbers.begin(), numbers.end()), numbers.end());
    
    for (int num : numbers) {
        tree.insert(num);
    }
    
    BOOST_CHECK_NO_THROW(tree.print());
    
    vector<int> toRemove;
    for (int i = 0; i < numbers.size(); i += 2) {
        toRemove.push_back(numbers[i]);
    }
    
    for (int num : toRemove) {
        BOOST_CHECK_NO_THROW(tree.remove(num));
    }

    for (int num : toRemove) {
        BOOST_CHECK(tree.find(num) == tree.nil);
    }
    
    for (int i = 1; i < numbers.size(); i += 2) {
        BOOST_CHECK(tree.find(numbers[i]) != tree.nil);
    }
}


BOOST_AUTO_TEST_SUITE_END()