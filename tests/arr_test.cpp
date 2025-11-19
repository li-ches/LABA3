#include "gtest/gtest.h"
#include "../arr.h"
#include "../serialize.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>

using namespace std;

// Хелпер для перехвата cout
class OutputCapture {
    std::stringstream buffer;
    std::streambuf* prev;
public:
    OutputCapture() : prev(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(prev); }
    std::string str() { return buffer.str(); }
};

TEST(MyArrTest, Branch_CapacityAndShrink) {
    MyArr arr;
    // ensureCapacity
    for(int i=0; i<65; ++i) {
        arr.addEnd("x");
    }
    EXPECT_GE(arr.lenArr(), 65);
    
    // Shrink: if (size * 4 <= capacity)
    int startSize = arr.lenArr();
    for(int i=0; i < startSize; ++i) {
        arr.delEnd(); 
    }
    EXPECT_EQ(arr.lenArr(), 0);
    
    // Shrink через delHead
    for(int i=0; i<20; ++i) arr.addEnd("y");
    for(int i=0; i<20; ++i) arr.delHead();
    EXPECT_EQ(arr.lenArr(), 0);

    // Shrink через delAt
    for(int i=0; i<20; ++i) arr.addEnd("z");
    for(int i=0; i<20; ++i) arr.delAt(0);
    EXPECT_EQ(arr.lenArr(), 0);
    
    // Доп. проверка shrink branch
    MyArr a;
    for (int i=0;i<40;++i) a.addEnd("x");
    for (int i=0;i<40;++i) a.delAt(0);
    EXPECT_EQ(a.lenArr(), 0);
}

TEST(MyArrTest, Branch_EdgeCases) {
    MyArr arr;
    OutputCapture cap; 

    // Пустой массив
    arr.readArray();
    arr.delHead();
    arr.delEnd();
    arr.delAt(0);
    arr.repArr(0, "val");
    
    // Неверные индексы
    arr.addEnd("A");
    arr.addAt(-1, "err");
    arr.addAt(5, "err");
    arr.delAt(-1);
    arr.delAt(5);
    arr.repArr(-1, "err");
    arr.repArr(5, "err");
    EXPECT_EQ(arr.getAt(-1), "[INVALID_INDEX]");
    EXPECT_EQ(arr.getAt(5), "[INVALID_INDEX]");

    // Вставка в начало/середину
    arr.addHead("Head");
    arr.addAt(1, "Mid"); 
    EXPECT_EQ(arr.getAt(0), "Head");
    EXPECT_EQ(arr.getAt(1), "Mid");

    // Геттеры
    EXPECT_EQ(arr.getAt(0), "Head");
    EXPECT_NE(arr.getData_test(), nullptr);
}

TEST(MyArrTest, FullCoverage) {
    MyArr arr;
    OutputCapture cap;
    
    // loadFromFile failure conditions
    arr.loadFromFile("non_exist.dat");
}

TEST(MyArrTest, SaveAndLoadFile) {
    OutputCapture cap;
    MyArr arr;
    
    arr.addEnd("hello");
    arr.addEnd("world");
    arr.saveToFile("arr_test.dat");
    
    MyArr arr2;
    arr2.loadFromFile("arr_test.dat");
    EXPECT_EQ(arr2.getAt(0), "hello");
    EXPECT_EQ(arr2.getAt(1), "world");
    
    remove("arr_test.dat");
}

TEST(SerializeTest, FailSafe) {
    MyArr arr;
    { ofstream f("bad.dat"); f << "x"; }
    arr.loadFromFile("bad.dat");
    remove("bad.dat");

    // 2. Truncated String
    { 
        ofstream f("trunc.dat", ios::binary);
        size_t len = 100;
        f.write((char*)&len, sizeof(len));
        // No data
    }
    {
        ifstream in("trunc.dat", ios::binary);
        readString(in);
    }
    remove("trunc.dat");
}