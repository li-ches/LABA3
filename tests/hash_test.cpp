#include "gtest/gtest.h"
#include "../hash.h"
#include <sstream>
#include <iostream>

class OutputCapture {
    std::stringstream buffer;
    std::streambuf* old;
public:
    OutputCapture() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { std::cout.rdbuf(old); }
    std::string str() const { return buffer.str(); }
};

//
// ========== ChainHash tests ==========
//

TEST(HashTest, ChainHash_Insert_Find_Erase) {
    ChainHash h(3);

    {   // insert A
        OutputCapture cap;
        h.insert("A","1");
        EXPECT_NE(cap.str().find("A"), std::string::npos);
    }

    {   // insert B (possible collision)
        OutputCapture cap;
        h.insert("B","2");
        EXPECT_NE(cap.str().find("цепочки"), std::string::npos);
    }

    {   // find existing
        OutputCapture cap;
        EXPECT_EQ(h.find("A"), "1");
        EXPECT_NE(cap.str().find("Найдено"), std::string::npos);
    }

    {   // find missing
        OutputCapture cap;
        EXPECT_EQ(h.find("XXX"), "");
        EXPECT_NE(cap.str().find("Не найдено"), std::string::npos);
    }

    {   // erase existing
        OutputCapture cap;
        EXPECT_TRUE(h.erase("A"));
        EXPECT_NE(cap.str().find("удален"), std::string::npos);
    }

    {   // erase missing
        OutputCapture cap;
        EXPECT_FALSE(h.erase("A"));
        EXPECT_NE(cap.str().find("не найден"), std::string::npos);
    }
}

//
// ========== OpenHash tests ==========
//

TEST(HashTest, OpenHash_Insert_Update_DeletedSlot) {
    OpenHash h(3);

    // Fill
    h.insert("A","1");
    h.insert("B","2");
    h.insert("C","3");

    {   // update A
        OutputCapture cap;
        h.insert("A","99");
        EXPECT_NE(cap.str().find("99"), std::string::npos);
    }

    {   // erase B
        OutputCapture cap;
        h.erase("B");
        EXPECT_NE(cap.str().find("удалено"), std::string::npos);
    }

    {   // insert into deleted slot
        OutputCapture cap;
        h.insert("X","Y");
        EXPECT_NE(cap.str().find("Y"), std::string::npos);
    }
}

TEST(HashTest, OpenHash_FindErase_NotFound) {
    OpenHash h(3);
    h.insert("A","1");

    {   // find missing
        OutputCapture cap;
        h.find("NOPE");
        EXPECT_NE(cap.str().find("Не найдено"), std::string::npos);
    }

    {   // erase missing
        OutputCapture cap;
        EXPECT_FALSE(h.erase("NOPE"));
        EXPECT_NE(cap.str().find("Ключ не найден"), std::string::npos);
    }
}

//
// ========== parse_cmd tests ==========
//

TEST(HashTest, parse_cmd_CorrectSplits) {
    std::string cmd, arg1, rest;

    parse_cmd("INSERT   key   value", cmd, arg1, rest);

    EXPECT_EQ(cmd, "INSERT");
    EXPECT_EQ(arg1, "key");
    EXPECT_EQ(rest, "value");
}

TEST(HashTest, parse_cmd_LeadingSpaces) {
    std::string cmd, arg1, rest;

    parse_cmd("   SEARCH   A", cmd, arg1, rest);

    // По вашей логике cmd будет = ""
    EXPECT_EQ(cmd, "");
    EXPECT_EQ(arg1, "SEARCH");
}

//
// ========== hash_man scripted interaction ==========
//

TEST(HashTest, hash_man_scripted) {
    std::stringstream fake_in;
    fake_in 
        << "INSERT A 1\n"
        << "SEARCH A\n"
        << "DELETE A\n"
        << "SHOW\n"
        << "BACK\n";

    std::streambuf* old_in = std::cin.rdbuf(fake_in.rdbuf());

    OutputCapture cap;
    hash_man();

    std::cin.rdbuf(old_in);

    EXPECT_NE(cap.str().find("УПРАВЛЕНИЕ"), std::string::npos);
    EXPECT_NE(cap.str().find("A"), std::string::npos);
    EXPECT_NE(cap.str().find("Выход"), std::string::npos);
}
