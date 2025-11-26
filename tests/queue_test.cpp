#include "gtest/gtest.h"
#include "../queue.h"
#include "../queue_serialize.h"
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

// проверка на основные операции очереди и граничные случаи
TEST(QueueTest, Branch_Coverage) {
    Queue q;
    OutputCapture cap;

    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.pop(), "[QUEUE_EMPTY]");
    EXPECT_EQ(q.peek(), "[QUEUE_EMPTY]");
    q.print();

    q.push("A");
    EXPECT_EQ(q.peek(), "A");
    EXPECT_EQ(q.pop(), "A"); 
    
    q.push("1");
    q.push("2");
    EXPECT_EQ(q.pop(), "1");
    EXPECT_EQ(q.pop(), "2");
}

// проверка на поведение при полном извлечении элементов из очереди
TEST(QueueTest, BranchBooster_PopEmpty) {
    OutputCapture cap;
    Queue q;
    
    q.push("A");
    q.push("B");
    q.pop(); 
    q.pop();
    EXPECT_TRUE(q.isEmpty());
}

// проверка на сохранение и загрузку очереди из файла
TEST(QueueTest, SaveAndLoadFile) {
    OutputCapture cap;
    Queue q;
    
    q.push("first");
    q.push("second");
    QueueSerializer::saveToFile(q, "queue_test.dat");
    
    Queue q2;
    QueueSerializer::loadFromFile(q2, "queue_test.dat");
    EXPECT_EQ(q2.pop(), "first");
    EXPECT_EQ(q2.pop(), "second");
    
    remove("queue_test.dat");
}

// проверка на загрузку из несуществующего файла
TEST(QueueTest, LoadFromBadFile) {
    OutputCapture cap;
    Queue q;
    QueueSerializer::loadFromFile(q, "nonexistent_queue.dat");
    EXPECT_TRUE(q.isEmpty());
}

// проверка на загрузку из обрезанного файла
TEST(QueueTest, TruncatedFile) {
    ofstream out("q_trunc.dat", ios::binary);
    int count = 2;
    out.write(reinterpret_cast<char*>(&count), sizeof(count));
    writeString(out, string("only"));
    out.close();

    Queue q; QueueSerializer::loadFromFile(q, "q_trunc.dat");
    remove("q_trunc.dat");
}

// проверка на корректность работы деструктора очереди
TEST(DestructorTest, QueueCleanup) {
    OutputCapture cap;
    {
        Queue q;
        q.push("1");
        q.push("2");
    }
}

// проверка на операции с пустой очередью
TEST(QueueTest, Coverage_EmptyOps) {
    Queue q;
    OutputCapture cap;
    
    EXPECT_EQ(q.pop(), "[QUEUE_EMPTY]");
    EXPECT_EQ(q.peek(), "[QUEUE_EMPTY]");
    
    q.print(); 
    EXPECT_NE(cap.str().find("пуста"), string::npos);
}

// проверка на полный цикл работы очереди (FIFO)
TEST(QueueTest, Coverage_FullCycle) {
    Queue q;
    q.push("A");
    q.push("B");
    
    EXPECT_EQ(q.peek(), "A");
    EXPECT_EQ(q.pop(), "A"); 
    
    EXPECT_EQ(q.peek(), "B");
    EXPECT_EQ(q.pop(), "B"); 
    
    EXPECT_TRUE(q.isEmpty());
}

// проверка на обработку ошибок ввода-вывода
TEST(QueueTest, Coverage_IO) {
    Queue q;
    QueueSerializer::saveToFile(q, "");
    QueueSerializer::loadFromFile(q, "missing.dat");
    
    {
        ofstream f("trunc.bin", ios::binary);
        int c = 5;
        f.write((char*)&c, sizeof(c));
        f.close();
    }
    QueueSerializer::loadFromBinaryFile(q, "trunc.bin");
    remove("trunc.bin");
}
// проверка вывода очереди с одним элементом
TEST(QueueTest, Print_SingleElement) {
    Queue q;
    q.push("single");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("single"), string::npos);
    EXPECT_EQ(output.find(" -> "), string::npos); // не должно быть стрелок для одного элемента
}

// проверка вывода очереди с несколькими элементами
TEST(QueueTest, Print_MultipleElements) {
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("first"), string::npos);
    EXPECT_NE(output.find("second"), string::npos);
    EXPECT_NE(output.find("third"), string::npos);
    EXPECT_NE(output.find(" -> "), string::npos); // должны быть стрелки между элементами
    
    // Проверяем порядок элементов (FIFO)
    size_t first_pos = output.find("first");
    size_t second_pos = output.find("second");
    size_t third_pos = output.find("third");
    EXPECT_LT(first_pos, second_pos); // first должен быть перед second
    EXPECT_LT(second_pos, third_pos); // second должен быть перед third
}

// проверка вывода пустой очереди
TEST(QueueTest, Print_EmptyQueue) {
    Queue q;
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь пуста."), string::npos);
    EXPECT_EQ(output.find("Очередь:"), string::npos); // не должно быть заголовка для пустой очереди
}

// проверка вывода очереди с двумя элементами (граничный случай для стрелок)
TEST(QueueTest, Print_TwoElements) {
    Queue q;
    q.push("first");
    q.push("last");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("first -> last"), string::npos); // должна быть одна стрелка
}

// проверка вывода очереди после операций pop
TEST(QueueTest, Print_AfterPopOperations) {
    Queue q;
    q.push("A");
    q.push("B");
    q.push("C");
    
    q.pop(); // удаляем A (первый элемент)
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("B -> C"), string::npos);
    EXPECT_EQ(output.find("A"), string::npos); // A не должно быть в выводе
}

// проверка вывода очереди с элементами содержащими специальные символы
TEST(QueueTest, Print_SpecialCharacters) {
    Queue q;
    q.push("element with spaces");
    q.push("element->with->arrows");
    q.push("normal");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("element with spaces"), string::npos);
    EXPECT_NE(output.find("element->with->arrows"), string::npos);
    EXPECT_NE(output.find("normal"), string::npos);
}

// проверка вывода очереди после полной очистки
TEST(QueueTest, Print_AfterClear) {
    Queue q;
    q.push("A");
    q.push("B");
    
    q.pop();
    q.pop();
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь пуста."), string::npos);
    EXPECT_EQ(output.find("Очередь:"), string::npos);
}

// проверка вывода очереди после последовательных push/pop операций
TEST(QueueTest, Print_AfterMixedOperations) {
    Queue q;
    q.push("A");
    q.push("B");
    q.pop(); // удаляем A
    q.push("C");
    q.push("D");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    // Ожидаемый порядок: B -> C -> D
    EXPECT_NE(output.find("B -> C -> D"), string::npos);
    EXPECT_EQ(output.find("A"), string::npos); // A не должно быть в выводе
}

// проверка вывода очереди с длинными строками
TEST(QueueTest, Print_LongStrings) {
    Queue q;
    q.push("very long string that might affect formatting");
    q.push("short");
    q.push("another long string with different content");
    
    OutputCapture cap;
    q.print();
    
    string output = cap.str();
    EXPECT_NE(output.find("Очередь:"), string::npos);
    EXPECT_NE(output.find("very long string"), string::npos);
    EXPECT_NE(output.find("short"), string::npos);
    EXPECT_NE(output.find("another long string"), string::npos);
}

// проверка вывода очереди после добавления в пустую очередь
TEST(QueueTest, Print_AfterPushToEmpty) {
    Queue q;
    
    OutputCapture cap1;
    q.print();
    string empty_output = cap1.str();
    EXPECT_NE(empty_output.find("Очередь пуста."), string::npos);
    
    q.push("new element");
    
    OutputCapture cap2;
    q.print();
    string non_empty_output = cap2.str();
    EXPECT_NE(non_empty_output.find("Очередь:"), string::npos);
    EXPECT_NE(non_empty_output.find("new element"), string::npos);
}