#include "gtest/gtest.h"
#include "../dlist.h"
#include "../dlist_serialize.h"
#include "../serialize.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class OutputCapture {
    stringstream buffer;
    streambuf* prev;
public:
    OutputCapture() : prev(cout.rdbuf(buffer.rdbuf())) {}
    ~OutputCapture() { cout.rdbuf(prev); }
    string str() { return buffer.str(); }
};

// проверка на основные операции добавления и удаления в двусвязном списке
TEST(DListTest, Branch_Killer) {
    DList d;

    d.addHead("A"); 
    d.addHead("B"); 

    d.delHead(); 
    EXPECT_EQ(d.getHead_Test()->value, "A");

    d.addTail("Z"); 
    d.delTail();
    EXPECT_EQ(d.getTail_Test()->value, "A");

    d.addTail("C"); 
    d.addAfter("A", "B"); 

    EXPECT_TRUE(d.contains("B"));
}

// проверка на операции с пустым списком
TEST(DListTest, Coverage_Empty_Ops) {
    DList d;
    d.delHead();
    d.delTail();
    d.delByVal("A");
    d.delAfterValue("A");
    d.delBeforeValue("A");
    d.addAfter("A", "B");
    d.addBefore("A", "B");
    
    EXPECT_FALSE(d.contains("A"));
}

// проверка на переходы между состояниями списка с одним элементом
TEST(DListTest, Coverage_SingleElement_Transitions) {
    DList d;
    
    d.addHead("A");
    d.delHead();
    EXPECT_EQ(d.getHead_Test(), nullptr);
    EXPECT_EQ(d.getTail_Test(), nullptr);
    
    d.addHead("B");
    d.delTail();
    EXPECT_EQ(d.getHead_Test(), nullptr);
    EXPECT_EQ(d.getTail_Test(), nullptr);
}

// проверка на сложные случаи вставки before/after
TEST(DListTest, Coverage_AddBefore_AddAfter_Complex) {
    DList d;
    d.addTail("A");
    d.addTail("C");
    
    d.addBefore("C", "B");
 
    OutputCapture cap;
    d.readForward();
    string out = cap.str();
    
    EXPECT_NE(out.find(" B "), string::npos); 
    
    d.addBefore("A", "Start");
    EXPECT_EQ(d.getHead_Test()->value, "Start");
    
    d.addAfter("C", "End");
    EXPECT_EQ(d.getTail_Test()->value, "End");
}

// проверка на различные ветви удаления по значению
TEST(DListTest, Coverage_DelByVal_Branches) {
    DList d;
    d.addTail("1");
    d.addTail("2");
    d.addTail("3");
    
    d.delByVal("2");
    EXPECT_FALSE(d.contains("2"));
    
    d.delByVal("1");
    EXPECT_EQ(d.getHead_Test()->value, "3");
    
    d.delByVal("3");
    EXPECT_EQ(d.getHead_Test(), nullptr);
    
    d.addTail("X");
    d.delByVal("Y");
    EXPECT_TRUE(d.contains("X"));
}

// проверка на граничные случаи удаления before/after
TEST(DListTest, Coverage_DelAfter_DelBefore_Boundary) {
    DList d;
    d.addTail("A");
    d.addTail("B");
    
    d.delBeforeValue("B"); 
    EXPECT_FALSE(d.contains("A"));
    EXPECT_EQ(d.getHead_Test()->value, "B");
    
    d.delBeforeValue("B"); 
    EXPECT_EQ(d.getHead_Test()->value, "B");
    
    d.delAfterValue("B"); 
    EXPECT_TRUE(d.contains("B"));
    
    d.addTail("C");
    d.delAfterValue("B"); 
    EXPECT_FALSE(d.contains("C"));
}

// проверка на обработку ошибок ввода-вывода
TEST(DListTest, Coverage_IO) {
    DList d;
    DListSerializer::saveToFile(d, ""); 
    DListSerializer::saveToBinaryFile(d, "");
    
    DListSerializer::loadFromFile(d, "missing.txt");
    DListSerializer::loadFromBinaryFile(d, "missing.bin");
    
    {
        ofstream f("bad.bin", ios::binary);
        int count = 5;
        f.write((char*)&count, sizeof(count));
        f.close();
    }
    DListSerializer::loadFromBinaryFile(d, "bad.bin");
    remove("bad.bin");
}
// проверка вывода обратного списка с одним элементом
TEST(DListTest, ReadBackward_SingleElement) {
    DList d;
    d.addTail("single");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    EXPECT_NE(output.find("single"), string::npos);
    EXPECT_EQ(output.find(" <-> "), string::npos); // не должно быть стрелок для одного элемента
}

// проверка вывода обратного списка с несколькими элементами
TEST(DListTest, ReadBackward_MultipleElements) {
    DList d;
    d.addTail("first");
    d.addTail("second");
    d.addTail("third");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    EXPECT_NE(output.find("third"), string::npos);
    EXPECT_NE(output.find("second"), string::npos);
    EXPECT_NE(output.find("first"), string::npos);
    EXPECT_NE(output.find(" <-> "), string::npos); // должны быть стрелки между элементами
    
    // Проверяем обратный порядок элементов
    size_t third_pos = output.find("third");
    size_t second_pos = output.find("second");
    size_t first_pos = output.find("first");
    EXPECT_LT(third_pos, second_pos); // third должен быть перед second (обратный порядок)
    EXPECT_LT(second_pos, first_pos); // second должен быть перед first
}

// проверка вывода пустого обратного списка
TEST(DListTest, ReadBackward_EmptyList) {
    DList d;
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список пуст."), string::npos);
    EXPECT_EQ(output.find("Список (обратный):"), string::npos); // не должно быть заголовка для пустого списка
}

// проверка вывода обратного списка с двумя элементами (граничный случай для стрелок)
TEST(DListTest, ReadBackward_TwoElements) {
    DList d;
    d.addTail("first");
    d.addTail("last");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    EXPECT_NE(output.find("last <-> first"), string::npos); // должна быть одна стрелка в обратном порядке
}

// проверка вывода обратного списка после операций удаления
TEST(DListTest, ReadBackward_AfterDeleteOperations) {
    DList d;
    d.addTail("A");
    d.addTail("B");
    d.addTail("C");
    d.addTail("D");
    
    d.delByVal("C"); // удаляем средний элемент
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    EXPECT_NE(output.find("D <-> B <-> A"), string::npos);
    EXPECT_EQ(output.find("C"), string::npos); // C не должно быть в выводе
}

// проверка вывода обратного списка с элементами содержащими специальные символы
TEST(DListTest, ReadBackward_SpecialCharacters) {
    DList d;
    d.addTail("element with spaces");
    d.addTail("element<->with<->arrows");
    d.addTail("normal");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    EXPECT_NE(output.find("element with spaces"), string::npos);
    EXPECT_NE(output.find("element<->with<->arrows"), string::npos);
    EXPECT_NE(output.find("normal"), string::npos);
}

// проверка вывода обратного списка после полной очистки
TEST(DListTest, ReadBackward_AfterClear) {
    DList d;
    d.addTail("A");
    d.addTail("B");
    
    d.delHead();
    d.delHead();
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список пуст."), string::npos);
    EXPECT_EQ(output.find("Список (обратный):"), string::npos);
}

// проверка корректности обратного порядка после добавления в голову
TEST(DListTest, ReadBackward_AfterAddHead) {
    DList d;
    d.addHead("C");
    d.addHead("B");
    d.addHead("A");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    // При добавлении в голову порядок в обратном списке должен быть: C -> B -> A
    EXPECT_NE(output.find("C <-> B <-> A"), string::npos);
}

// проверка корректности обратного порядка после смешанных операций
TEST(DListTest, ReadBackward_MixedOperations) {
    DList d;
    d.addTail("A");
    d.addHead("Start");
    d.addTail("Z");
    d.addAfter("A", "Middle");
    
    OutputCapture cap;
    d.readBackward();
    
    string output = cap.str();
    EXPECT_NE(output.find("Список (обратный):"), string::npos);
    // Ожидаемый обратный порядок: Z -> Middle -> A -> Start
    EXPECT_NE(output.find("Z"), string::npos);
    EXPECT_NE(output.find("Middle"), string::npos);
    EXPECT_NE(output.find("A"), string::npos);
    EXPECT_NE(output.find("Start"), string::npos);
}