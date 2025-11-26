#include "gtest/gtest.h"
#include "../stack.h"
#include "../stack_serialize.h"
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

// проверка на основные операции стека и граничные случаи
TEST(StackTest, Branch_Coverage) {
    Stack s;
    OutputCapture cap;

    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    s.readStack();

    s.push("A");
    EXPECT_EQ(s.peek(), "A");
    EXPECT_EQ(s.pop(), "A");
}

// проверка на расширенные сценарии работы стека (LIFO)
TEST(StackTest, Branch_Killer_And_Extras) {
    Stack s;
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    
    OutputCapture cap; 
    s.readStack();

    s.push("one"); s.push("two"); s.push("three");
    EXPECT_EQ(s.pop(), "three");
    EXPECT_EQ(s.pop(), "two");
    EXPECT_EQ(s.pop(), "one");
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
}

// проверка на сохранение и загрузку стека из файла
TEST(StackTest, SaveAndLoadFile) {
    OutputCapture cap;
    Stack s;
    
    s.push("hello");
    s.push("world");
    StackSerializer::saveToFile(s, "stack_test.dat");
    
    Stack s2;
    StackSerializer::loadFromFile(s2, "stack_test.dat");
    EXPECT_EQ(s2.pop(), "world");
    EXPECT_EQ(s2.pop(), "hello");
    
    remove("stack_test.dat");
}

// проверка на загрузку из несуществующих и поврежденных файлов
TEST(StackTest, LoadFromBadFile) {
    OutputCapture cap;
    Stack s;
    StackSerializer::loadFromFile(s, "nonexistent_file.dat");
    EXPECT_TRUE(s.isEmpty());
    
    { ofstream f("bad_stack.dat", ios::binary); f << "x"; }
    StackSerializer::loadFromFile(s, "bad_stack.dat");
    remove("bad_stack.dat");
}

// проверка на корректность работы деструктора стека
TEST(DestructorTest, StackCleanup) {
    OutputCapture cap;
    {
        Stack s;
        s.push("1");
        s.push("2");
    }
}

// проверка на операции с пустым стеком
TEST(StackTest, Coverage_EmptyOps) {
    Stack s;
    OutputCapture cap;
    
    EXPECT_EQ(s.pop(), "[STACK_EMPTY]");
    EXPECT_EQ(s.peek(), "[STACK_EMPTY]");
    
    s.readStack();
    EXPECT_NE(cap.str().find("пуст"), string::npos);
}

// проверка на обработку ошибок ввода-вывода
TEST(StackTest, Coverage_IO) {
    Stack s;
    StackSerializer::saveToFile(s, "");
    StackSerializer::loadFromFile(s, "missing.dat");
    
    {
        ofstream f("bad_stack.bin", ios::binary);
        int c = 10;
        f.write((char*)&c, sizeof(c));
        f.close();
    }
    StackSerializer::loadFromBinaryFile(s, "bad_stack.bin");
    remove("bad_stack.bin");
}
// проверка вывода стека с одним элементом
TEST(StackTest, ReadStack_SingleElement) {
    Stack s;
    s.push("single");
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек (верх -> низ):"), string::npos);
    EXPECT_NE(output.find("single"), string::npos);
    EXPECT_EQ(output.find(" -> "), string::npos); // не должно быть стрелок для одного элемента
}

// проверка вывода стека с несколькими элементами
TEST(StackTest, ReadStack_MultipleElements) {
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек (верх -> низ):"), string::npos);
    EXPECT_NE(output.find("third"), string::npos);
    EXPECT_NE(output.find("second"), string::npos);
    EXPECT_NE(output.find("first"), string::npos);
    EXPECT_NE(output.find(" -> "), string::npos); // должны быть стрелки между элементами
    
    // Проверяем порядок элементов (LIFO)
    size_t third_pos = output.find("third");
    size_t second_pos = output.find("second");
    size_t first_pos = output.find("first");
    EXPECT_LT(third_pos, second_pos); // third должен быть перед second
    EXPECT_LT(second_pos, first_pos); // second должен быть перед first
}

// проверка вывода пустого стека
TEST(StackTest, ReadStack_EmptyStack) {
    Stack s;
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек пуст."), string::npos);
    EXPECT_EQ(output.find("Стек (верх -> низ):"), string::npos); // не должно быть заголовка для пустого стека
}

// проверка вывода стека с двумя элементами (граничный случай для стрелок)
TEST(StackTest, ReadStack_TwoElements) {
    Stack s;
    s.push("bottom");
    s.push("top");
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек (верх -> низ):"), string::npos);
    EXPECT_NE(output.find("top -> bottom"), string::npos); // должна быть одна стрелка
}

// проверка вывода стека после операций pop
TEST(StackTest, ReadStack_AfterPopOperations) {
    Stack s;
    s.push("A");
    s.push("B");
    s.push("C");
    
    s.pop(); // удаляем C
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек (верх -> низ):"), string::npos);
    EXPECT_NE(output.find("B -> A"), string::npos);
    EXPECT_EQ(output.find("C"), string::npos); // C не должно быть в выводе
}

// проверка вывода стека с элементами содержащими специальные символы
TEST(StackTest, ReadStack_SpecialCharacters) {
    Stack s;
    s.push("element with spaces");
    s.push("element->with->arrows");
    s.push("normal");
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек (верх -> низ):"), string::npos);
    EXPECT_NE(output.find("element with spaces"), string::npos);
    EXPECT_NE(output.find("element->with->arrows"), string::npos);
    EXPECT_NE(output.find("normal"), string::npos);
}

// проверка вывода стека после полной очистки
TEST(StackTest, ReadStack_AfterClear) {
    Stack s;
    s.push("A");
    s.push("B");
    
    s.pop();
    s.pop();
    
    OutputCapture cap;
    s.readStack();
    
    string output = cap.str();
    EXPECT_NE(output.find("Стек пуст."), string::npos);
    EXPECT_EQ(output.find("Стек (верх -> низ):"), string::npos);
}