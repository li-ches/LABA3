#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include "dynarr.h"
#include "onelist.h"
#include "twolist.h"
#include "stack.h"
#include "queue.h"
#include "rbtree.h"
#include "hash-table.h"
#include <functional>

using namespace std;

using CommandHandler = function<void(const vector<string>&)>;
map<string, CommandHandler> commandHandlers;

map<string, DynArr*> arrays;
map<string, OneList*> oneLists;
map<string, TwoList*> twoLists;
map<string, Stack*> stacks;
map<string, Queue*> queues;
map<string, RBTree*> trees;
map<string, DoubleHashTable*> doubleHashTables;
map<string, CuckooHashTable*> cuckooHashTables;
string currFilename;

void saveToFile() {
    if (currFilename.empty()) {
        return;
    }
    ofstream file(currFilename);
    if (!file.is_open()) {
        throw runtime_error("Ошибка: не удалось создать файл");
    }
    
    for (const auto& arr : arrays) {
        file << "ARRAY " << arr.first << " ";
        arr.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& list : oneLists) {
        file << "ONELIST " << list.first << " ";
        list.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& list : twoLists) {
        file << "TWOLIST " << list.first << " ";
        list.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& stack : stacks) {
        file << "STACK " << stack.first << " ";
        stack.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& queue : queues) {
        file << "QUEUE " << queue.first << " ";
        queue.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& tree : trees) {
        file << "TREE " << tree.first << " ";
        tree.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& hashTable : doubleHashTables) {
        file << "DOUBLEHASH " << hashTable.first << " ";
        hashTable.second->saveToFile(file);
        file << '\n';
    }
    
    for (const auto& hashTable : cuckooHashTables) {
        file << "CUCKOO " << hashTable.first << " ";
        hashTable.second->saveToFile(file);
        file << '\n';
    }
    
    file.close();
}


void saveToBinaryFile() {
    if (currFilename.empty()) {
        throw runtime_error("Ошибка: имя файла не установлено");
    }
    
    ofstream file(currFilename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Ошибка: не удалось создать файл");
    }
    
    for (const auto& arr : arrays) {
        char type = 'A'; // ARRAY
        file.write(&type, sizeof(type));
        
        size_t nameSize = arr.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(arr.first.c_str(), static_cast<streamsize>(nameSize));
        arr.second->saveToBinaryFile(file);
    }
    
    for (const auto& list : oneLists) {
        char type = 'F'; // ONELIST
        file.write(&type, sizeof(type));
        
        size_t nameSize = list.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(list.first.c_str(), static_cast<streamsize>(nameSize));
        list.second->saveToBinaryFile(file);
    }
    
    for (const auto& list : twoLists) {
        char type = 'L'; // TWOLIST
        file.write(&type, sizeof(type));
        
        size_t nameSize = list.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(list.first.c_str(), static_cast<streamsize>(nameSize));
        list.second->saveToBinaryFile(file);
    }
    
    for (const auto& stack : stacks) {
        char type = 'S'; // STACK
        file.write(&type, sizeof(type));
        
        size_t nameSize = stack.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(stack.first.c_str(), static_cast<streamsize>(nameSize));
        stack.second->saveToBinaryFile(file);
    }
    
    for (const auto& queue : queues) {
        char type = 'Q'; // QUEUE
        file.write(&type, sizeof(type));
        
        size_t nameSize = queue.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(queue.first.c_str(), static_cast<streamsize>(nameSize));
        queue.second->saveToBinaryFile(file);
    }
    
    for (const auto& tree : trees) {
        char type = 'T'; // TREE
        file.write(&type, sizeof(type));
        
        size_t nameSize = tree.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(tree.first.c_str(), static_cast<streamsize>(nameSize));
        tree.second->saveToBinaryFile(file);
    }
    
    for (const auto& hashTable : doubleHashTables) {
        char type = 'D'; // DOUBLEHASH
        file.write(&type, sizeof(type));
        
        size_t nameSize = hashTable.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(hashTable.first.c_str(), static_cast<streamsize>(nameSize));
        hashTable.second->saveToBinaryFile(file);
    }
    
    for (const auto& hashTable : cuckooHashTables) {
        char type = 'C'; // CUCKOO
        file.write(&type, sizeof(type));
        
        size_t nameSize = hashTable.first.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(hashTable.first.c_str(), static_cast<streamsize>(nameSize));
        hashTable.second->saveToBinaryFile(file);
    }
    
    file.close();
}

void loadStructure(const string& type, const string& name, stringstream& dataStream) {
    string tempFilename = "temp_" + name + ".txt";
    ofstream tempFile(tempFilename);
    tempFile << dataStream.str();
    tempFile.close();

    ifstream file(tempFilename);
    if (type == "ARRAY") {
        if (arrays.find(name) != arrays.end()){ 
            delete arrays[name];
        }
        arrays[name] = new DynArr();
        arrays[name]->loadFromFile(file);
    }
    if (type == "ONELIST") {
        if (oneLists.find(name) != oneLists.end()){
            delete oneLists[name];
        }
        oneLists[name] = new OneList();
        oneLists[name]->loadFromFile(file);
    }
    if (type == "TWOLIST") {
        if (twoLists.find(name) != twoLists.end()){ 
            delete twoLists[name];
        }
        twoLists[name] = new TwoList();
        twoLists[name]->loadFromFile(file);
    }
    if (type == "STACK") {
        if (stacks.find(name) != stacks.end()){ 
            delete stacks[name];
        }
        stacks[name] = new Stack();
        stacks[name]->loadFromFile(file);
    }
    if (type == "QUEUE") {
        if (queues.find(name) != queues.end()){ 
            delete queues[name];
        }
        queues[name] = new Queue();
        queues[name]->loadFromFile(file);
    }
    if (type == "TREE") {
        if (trees.find(name) != trees.end()){ 
            delete trees[name];
        }
        trees[name] = new RBTree();
        trees[name]->loadFromFile(file);
    }
    if (type == "DOUBLEHASH") {
        if (doubleHashTables.find(name) != doubleHashTables.end()){ 
            delete doubleHashTables[name];
        }
        doubleHashTables[name] = new DoubleHashTable();
        doubleHashTables[name]->loadFromFile(file);
    }
    if (type == "CUCKOO") {
        if (cuckooHashTables.find(name) != cuckooHashTables.end()){ 
            delete cuckooHashTables[name];
        }
        cuckooHashTables[name] = new CuckooHashTable();
        cuckooHashTables[name]->loadFromFile(file);
    }
    
    file.close();
    remove(tempFilename.c_str()); 
}

void loadFromFile() {
    ifstream file(currFilename);
    if (!file.is_open()){ 
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream sss(line);
        string type;
        string name;
        sss >> type >> name;
        
        string data;
        getline(sss, data);
        stringstream dataStream(data);
        
        loadStructure(type, name, dataStream);
    }  
    file.close();
}

void loadFromBinaryFile() {
    if (currFilename.empty()) {
        return;
    }
    
    ifstream file(currFilename, ios::binary);
    if (!file.is_open()) {
        return;
    }
    for (auto& arr : arrays){ 
        delete arr.second;
    }
    for (auto& list : oneLists){ 
        delete list.second;
    }
    for (auto& list : twoLists){ 
        delete list.second;
    }
    for (auto& stack : stacks){ 
        delete stack.second;
    }
    for (auto& queue : queues){ 
        delete queue.second;
    }
    for (auto& tree : trees){ 
        delete tree.second;
    }
    for (auto& hashTable : doubleHashTables){ 
        delete hashTable.second;
    }
    for (auto& hashTable : cuckooHashTables){ 
        delete hashTable.second;
    }
    arrays.clear();
    oneLists.clear();
    twoLists.clear();
    stacks.clear();
    queues.clear();
    trees.clear();
    doubleHashTables.clear();
    cuckooHashTables.clear();
    
    while (file.peek() != EOF) {

        char type;
        file.read(&type, sizeof(type));
        if (file.eof()){ 
            break;
        }

        size_t nameSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        if (file.eof()){ 
            break;
        }

        string name(nameSize, ' ');
        file.read(name.data(), static_cast<streamsize>(nameSize));
        if (file.eof()){ 
            break;
        }
        
        switch (type) {
            case 'A': { // ARRAY
                arrays[name] = new DynArr();
                arrays[name]->loadFromBinaryFile(file);
                break;
            }
            case 'F': { // ONELIST
                oneLists[name] = new OneList();
                oneLists[name]->loadFromBinaryFile(file);
                break;
            }
            case 'L': { // TWOLIST
                twoLists[name] = new TwoList();
                twoLists[name]->loadFromBinaryFile(file);
                break;
            }
            case 'S': { // STACK
                stacks[name] = new Stack();
                stacks[name]->loadFromBinaryFile(file);
                break;
            }
            case 'Q': { // QUEUE
                queues[name] = new Queue();
                queues[name]->loadFromBinaryFile(file);
                break;
            }
            case 'T': { // TREE
                trees[name] = new RBTree();
                trees[name]->loadFromBinaryFile(file);
                break;
            }
            case 'D': { // DOUBLEHASH
                doubleHashTables[name] = new DoubleHashTable();
                doubleHashTables[name]->loadFromBinaryFile(file);
                break;
            }
            case 'C': { // CUCKOO
                cuckooHashTables[name] = new CuckooHashTable();
                cuckooHashTables[name]->loadFromBinaryFile(file);
                break;
            }
            default:
                // Неизвестный тип, пропускаем
                break;
        }
    }
    
    file.close();
}

void MCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (arrays.find(name) != arrays.end()) {
        delete arrays[name];
    }
    arrays[name] = new DynArr();
    saveToFile();
}

void BMCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (arrays.find(name) != arrays.end()) {
        delete arrays[name];
    }
    arrays[name] = new DynArr();
    saveToBinaryFile();
}

void FCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) != oneLists.end()) {
        delete oneLists[name];
    }
    oneLists[name] = new OneList();
    saveToFile();
}

void BFCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) != oneLists.end()) {
        delete oneLists[name];
    }
    oneLists[name] = new OneList();
    saveToBinaryFile();
}

void LCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) != twoLists.end()) {
        delete twoLists[name];
    }
    twoLists[name] = new TwoList();
    saveToFile();
}

void BLCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) != twoLists.end()) {
        delete twoLists[name];
    }
    twoLists[name] = new TwoList();
    saveToBinaryFile();
}

void SCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (stacks.find(name) != stacks.end()) {
        delete stacks[name];
    }
    stacks[name] = new Stack();
    saveToFile();
}

void BSCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (stacks.find(name) != stacks.end()) {
        delete stacks[name];
    }
    stacks[name] = new Stack();
    saveToBinaryFile();
}

void QCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (queues.find(name) != queues.end()) {
        delete queues[name];
    }
    queues[name] = new Queue();
    saveToFile();
}

void BQCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (queues.find(name) != queues.end()) {
        delete queues[name];
    }
    queues[name] = new Queue();
    saveToBinaryFile();
}

void TCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (trees.find(name) != trees.end()) {
        delete trees[name];
    }
    trees[name] = new RBTree();
    saveToFile();
}

void BTCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (trees.find(name) != trees.end()) {
        delete trees[name];
    }
    trees[name] = new RBTree();
    saveToBinaryFile();
}

void DCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (doubleHashTables.find(name) != doubleHashTables.end()) {
        delete doubleHashTables[name];
    }
    doubleHashTables[name] = new DoubleHashTable();
    saveToFile();
}

void BDCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (doubleHashTables.find(name) != doubleHashTables.end()) {
        delete doubleHashTables[name];
    }
    doubleHashTables[name] = new DoubleHashTable();
    saveToBinaryFile();
}

void CCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (cuckooHashTables.find(name) != cuckooHashTables.end()) {
        delete cuckooHashTables[name];
    }
    cuckooHashTables[name] = new CuckooHashTable();
    saveToFile();
}

void BCCREATE(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (cuckooHashTables.find(name) != cuckooHashTables.end()) {
        delete cuckooHashTables[name];
    }
    cuckooHashTables[name] = new CuckooHashTable();
    saveToBinaryFile();
}

void MREPLACE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    const string& value = param[3];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    
    arrays[name]->replaceElement(index, value);
    saveToFile();
}

void BMREPLACE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    const string& value = param[3];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    
    arrays[name]->replaceElement(index, value);
    saveToBinaryFile();
}

void MPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->addToEnd(value);
    saveToFile();
}

void BMPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->addToEnd(value);
    saveToBinaryFile();
}

void MPUSHINDEX(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    const string& value = param[3];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->addAtIndex(index, value);
    saveToFile();
}

void BMPUSHINDEX(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    const string& value = param[3];
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->addAtIndex(index, value);
    saveToBinaryFile();
}

void MDEL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->removeAtIndex(index);
    saveToFile();
}

void BMDEL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    arrays[name]->removeAtIndex(index);
    saveToBinaryFile();
}

void MGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    cout << arrays[name]->getElement(index) << '\n';
}

void BMGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int index = stoi(param[2]);
    
    if (arrays.find(name) == arrays.end()) {
        throw runtime_error("Ошибка: массив не найден");
    }
    cout << arrays[name]->getElement(index) << '\n';
}

void FPUSHHEAD(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addToHead(value);
    saveToFile();
}

void BFPUSHHEAD(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addToHead(value);
    saveToBinaryFile();
}

void FPUSHTAIL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addToTail(value);
    saveToFile();
}

void BFPUSHTAIL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addToTail(value);
    saveToBinaryFile();
}

void FPUSHAFTER(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addAfterValue(targetVal, newVal);
    saveToFile();
}

void BFPUSHAFTER(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addAfterValue(targetVal, newVal);
    saveToBinaryFile();
}

void FPUSHBEFORE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addBeforeValue(targetVal, newVal);
    saveToFile();
}

void BFPUSHBEFORE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->addBeforeValue(targetVal, newVal);
    saveToBinaryFile();
}

void FDELHEAD(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeFromHead();
    saveToFile();
}

void BFDELHEAD(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeFromHead();
    saveToBinaryFile();
}

void FDELTAIL(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeFromTail();
    saveToFile();
}

void BFDELTAIL(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeFromTail();
    saveToBinaryFile();
}

void FDELAFTER(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeAfterValue(targetVal);
    saveToFile();
}

void BFDELAFTER(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeAfterValue(targetVal);
    saveToBinaryFile();
}

void FDELBEFORE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeBeforeValue(targetVal);
    saveToFile();
}

void BFDELBEFORE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeBeforeValue(targetVal);
    saveToBinaryFile();
}

void FDELVALUE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeByValue(targetVal);
    saveToFile();
}

void BFDELVALUE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    oneLists[name]->removeByValue(targetVal);
    saveToBinaryFile();
}

void FGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    cout << oneLists[name]->getElementByValue(targetVal) << '\n';
}

void BFGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (oneLists.find(name) == oneLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    cout << oneLists[name]->getElementByValue(targetVal) << '\n';
}

void LPUSHHEAD(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addToHead(value);
    saveToFile();
}

void BLPUSHHEAD(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addToHead(value);
    saveToBinaryFile();
}

void LPUSHTAIL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addToTail(value);
    saveToFile();
}

void BLPUSHTAIL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addToTail(value);
    saveToBinaryFile();
}

void LPUSHAFTER(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addAfterValue(targetVal, newVal);
    saveToFile();
}

void BLPUSHAFTER(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addAfterValue(targetVal, newVal);
    saveToBinaryFile();
}

void LPUSHBEFORE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addBeforeValue(targetVal, newVal);
    saveToFile();
}

void BLPUSHBEFORE(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    const string& newVal = param[3];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->addBeforeValue(targetVal, newVal);
    saveToBinaryFile();
}

void LDELHEAD(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeFromHead();
    saveToFile();
}

void BLDELHEAD(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeFromHead();
    saveToBinaryFile();
}

void LDELTAIL(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeFromTail();
    saveToFile();
}

void BLDELTAIL(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeFromTail();
    saveToBinaryFile();
}

void LDELAFTER(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeAfterValue(targetVal);
    saveToFile();
}

void BLDELAFTER(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];

    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeAfterValue(targetVal);
    saveToBinaryFile();
}

void LDELBEFORE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeBeforeValue(targetVal);
    saveToFile();
}

void BLDELBEFORE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeBeforeValue(targetVal);
    saveToBinaryFile();
}

void LDELVALUE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeByValue(targetVal);
    saveToFile();
}

void BLDELVALUE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    twoLists[name]->removeByValue(targetVal);
    saveToBinaryFile();
}

void LGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    cout << twoLists[name]->getElementByValue(targetVal) << '\n';
}

void BLGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& targetVal = param[2];
    
    if (twoLists.find(name) == twoLists.end()) {
        throw runtime_error("Ошибка: список не найден");
    }
    cout << twoLists[name]->getElementByValue(targetVal) << '\n';
}

void SPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (stacks.find(name) == stacks.end()) {
        throw runtime_error("Ошибка: стек не найден");
    }
    stacks[name]->push(value);
    saveToFile();
}

void BSPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (stacks.find(name) == stacks.end()) {
        throw runtime_error("Ошибка: стек не найден");
    }
    stacks[name]->push(value);
    saveToBinaryFile();
}

void SPOP(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (stacks.find(name) == stacks.end()) {
        throw runtime_error("Ошибка: стек не найден");
    }
    stacks[name]->pop();
    saveToFile();
}

void BSPOP(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (stacks.find(name) == stacks.end()) {
        throw runtime_error("Ошибка: стек не найден");
    }
    stacks[name]->pop();
    saveToBinaryFile();
}

void QPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (queues.find(name) == queues.end()) {
        throw runtime_error("Ошибка: очередь не найдена");
    }
    queues[name]->enqueue(value);
    saveToFile();
}

void BQPUSH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& value = param[2];
    
    if (queues.find(name) == queues.end()) {
        throw runtime_error("Ошибка: очередь не найдена");
    }
    queues[name]->enqueue(value);
    saveToBinaryFile();
}

void QPOP(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (queues.find(name) == queues.end()) {
        throw runtime_error("Ошибка: очередь не найдена");
    }
    queues[name]->dequeue();
    saveToFile();
}

void BQPOP(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (queues.find(name) == queues.end()) {
        throw runtime_error("Ошибка: очередь не найдена");
    }
    queues[name]->dequeue();
    saveToBinaryFile();
}

void TINSERT(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    trees[name]->insert(value);
    saveToFile();
}

void BTINSERT(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    trees[name]->insert(value);
    saveToBinaryFile();
}

void TDEL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    trees[name]->remove(value);
    saveToFile();
}

void BTDEL(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    trees[name]->remove(value);
    saveToBinaryFile();
}

void TGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    RBTreeNode* node = trees[name]->find(value);
    if (node == trees[name]->nil) {
        throw runtime_error("Ошибка: значение не найдено в дереве");
    }
    cout << node->data << '\n';
}

void BTGET(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    int value = stoi(param[2]); 
    
    if (trees.find(name) == trees.end()) {
        throw runtime_error("Ошибка: дерево не найдено");
    }
    RBTreeNode* node = trees[name]->find(value);
    if (node == trees[name]->nil) {
        throw runtime_error("Ошибка: значение не найдено в дереве");
    }
    cout << node->data << '\n';
}

void DINSERT(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    string value = param[3];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!doubleHashTables[name]->insert(key, value)) {
        throw runtime_error("Ошибка: не удалось вставить элемент");
    }
    saveToFile();
}

void BDINSERT(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    string value = param[3];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!doubleHashTables[name]->insert(key, value)) {
        throw runtime_error("Ошибка: не удалось вставить элемент");
    }
    saveToBinaryFile();
}

void DSEARCH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    string result = doubleHashTables[name]->search(key);
    if (result.empty()) {  
        throw runtime_error("Ошибка: ключ не найден");
    }
    cout << result << '\n';
}

void BDSEARCH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    string result = doubleHashTables[name]->search(key);
    if (result.empty()) {  
        throw runtime_error("Ошибка: ключ не найден");
    }
    cout << result << '\n';
}

void DREMOVE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!doubleHashTables[name]->remove(key)) {
        throw runtime_error("Ошибка: не удалось удалить элемент");
    }
    saveToFile();
}

void BDREMOVE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (doubleHashTables.find(name) == doubleHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!doubleHashTables[name]->remove(key)) {
        throw runtime_error("Ошибка: не удалось удалить элемент");
    }
    saveToBinaryFile();
}

void CINSERT(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    string value = param[3];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!cuckooHashTables[name]->insert(key, value)) {
        throw runtime_error("Ошибка: не удалось вставить элемент");
    }
    saveToFile();
}

void BCINSERT(const vector<string>& param) {
    if (param.size() < 4) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    string value = param[3];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!cuckooHashTables[name]->insert(key, value)) {
        throw runtime_error("Ошибка: не удалось вставить элемент");
    }
    saveToBinaryFile();
}

void CSEARCH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    string result = cuckooHashTables[name]->search(key);
    if (result.empty()) {  
        throw runtime_error("Ошибка: ключ не найден");
    }
    cout << result << '\n';
}

void BCSEARCH(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    string result = cuckooHashTables[name]->search(key);
    if (result.empty()) {  
        throw runtime_error("Ошибка: ключ не найден");
    }
    cout << result << '\n';
}

void CREMOVE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!cuckooHashTables[name]->remove(key)) {
        throw runtime_error("Ошибка: не удалось удалить элемент");
    }
    saveToFile();
}

void BCREMOVE(const vector<string>& param) {
    if (param.size() < 3) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    const string& key = param[2];
    
    if (cuckooHashTables.find(name) == cuckooHashTables.end()) {
        throw runtime_error("Ошибка: хэш-таблица не найдена");
    }
    if (!cuckooHashTables[name]->remove(key)) {
        throw runtime_error("Ошибка: не удалось удалить элемент");
    }
    saveToBinaryFile();
}

void PRINT(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (arrays.find(name) != arrays.end()) {
        arrays[name]->printArr();
    } else if (oneLists.find(name) != oneLists.end()) {
        oneLists[name]->print();
    } else if (twoLists.find(name) != twoLists.end()) {
        twoLists[name]->print();
    } else if (stacks.find(name) != stacks.end()) {
        stacks[name]->print();
    } else if (queues.find(name) != queues.end()) {
        queues[name]->print();
    } else if (trees.find(name) != trees.end()) {
        trees[name]->print();
    } else if (doubleHashTables.find(name) != doubleHashTables.end()) {
        doubleHashTables[name]->print();
    } else if (cuckooHashTables.find(name) != cuckooHashTables.end()) {
        cuckooHashTables[name]->print();
    } else {
        throw runtime_error("Ошибка: структура не найдена");
    }
}

void BPRINT(const vector<string>& param) {
    if (param.size() < 2) {
        throw runtime_error("Ошибка: недостаточно параметров");
    }
    const string& name = param[1];
    
    if (arrays.find(name) != arrays.end()) {
        arrays[name]->printArr();
    } else if (oneLists.find(name) != oneLists.end()) {
        oneLists[name]->print();
    } else if (twoLists.find(name) != twoLists.end()) {
        twoLists[name]->print();
    } else if (stacks.find(name) != stacks.end()) {
        stacks[name]->print();
    } else if (queues.find(name) != queues.end()) {
        queues[name]->print();
    } else if (trees.find(name) != trees.end()) {
        trees[name]->print();
    } else if (doubleHashTables.find(name) != doubleHashTables.end()) {
        doubleHashTables[name]->print();
    } else if (cuckooHashTables.find(name) != cuckooHashTables.end()) {
        cuckooHashTables[name]->print();
    } else {
        throw runtime_error("Ошибка: структура не найдена");
    }
}

void HELP([[maybe_unused]] const vector<string>& param) {
    cout << "Доступные команды:" << '\n';
    cout << "Текстовые команды (префикс без B):" << '\n';
    cout << "  MCREATE, FCREATE, LCREATE, SCREATE, QCREATE, TCREATE, DCREATE, CCREATE" << '\n';
    cout << "  MPUSH, MPUSHINDEX, MREPLACE, MDEL, MGET" << '\n';
    cout << "  FPUSHHEAD, FPUSHTAIL, FPUSHAFTER, FPUSHBEFORE" << '\n';
    cout << "  FDELHEAD, FDELTAIL, FDELAFTER, FDELBEFORE, FDELVALUE, FGET" << '\n';
    cout << "  LPUSHHEAD, LPUSHTAIL, LPUSHAFTER, LPUSHBEFORE" << '\n';
    cout << "  LDELHEAD, LDELTAIL, LDELAFTER, LDELBEFORE, LDELVALUE, LGET" << '\n';
    cout << "  SPUSH, SPOP, QPUSH, QPOP" << '\n';
    cout << "  TINSERT, TDEL, TGET" << '\n';
    cout << "  DINSERT, DSEARCH, DREMOVE" << '\n';
    cout << "  CINSERT, CSEARCH, CREMOVE" << '\n';
    cout << "  PRINT, HELP" << '\n';
    cout << '\n' << "Бинарные команды (префикс B):" << '\n';
    cout << "  BMCREATE, BFCREATE, BLCREATE, BSCREATE, BQCREATE, BTCREATE, BDCREATE, BCCREATE" << '\n';
    cout << "  BMPUSH, BMPUSHINDEX, BMREPLACE, BMDEL, BMGET" << '\n';
    cout << "  BFPUSHHEAD, BFPUSHTAIL, BFPUSHAFTER, BFPUSHBEFORE" << '\n';
    cout << "  BFDELHEAD, BFDELTAIL, BFDELAFTER, BFDELBEFORE, BFDELVALUE, BFGET" << '\n';
    cout << "  BLPUSHHEAD, BLPUSHTAIL, BLPUSHAFTER, BLPUSHBEFORE" << '\n';
    cout << "  BLDELHEAD, BLDELTAIL, BLDELAFTER, BLDELBEFORE, BLDELVALUE, BLGET" << '\n';
    cout << "  BSPUSH, BSPOP, BQPUSH, BQPOP" << '\n';
    cout << "  BTINSERT, BTDEL, BTGET" << '\n';
    cout << "  BDINSERT, BDSEARCH, BDREMOVE" << '\n';
    cout << "  BCINSERT, BCSEARCH, BCREMOVE" << '\n';
    cout << "  BPRINT" << '\n';
}

void initializeCommands() {
    commandHandlers["MCREATE"] = MCREATE;
    commandHandlers["BMCREATE"] = BMCREATE;
    commandHandlers["MPUSH"] = MPUSH;
    commandHandlers["BMPUSH"] = BMPUSH;
    commandHandlers["MPUSHINDEX"] = MPUSHINDEX;
    commandHandlers["BMPUSHINDEX"] = BMPUSHINDEX;
    commandHandlers["MREPLACE"] = MREPLACE;
    commandHandlers["BMREPLACE"] = BMREPLACE;
    commandHandlers["MDEL"] = MDEL;
    commandHandlers["BMDEL"] = BMDEL;
    commandHandlers["MGET"] = MGET;
    commandHandlers["BMGET"] = BMGET;

    commandHandlers["FCREATE"] = FCREATE;
    commandHandlers["BFCREATE"] = BFCREATE;
    commandHandlers["FPUSHHEAD"] = FPUSHHEAD;
    commandHandlers["BFPUSHHEAD"] = BFPUSHHEAD;
    commandHandlers["FPUSHTAIL"] = FPUSHTAIL;
    commandHandlers["BFPUSHTAIL"] = BFPUSHTAIL;
    commandHandlers["FPUSHAFTER"] = FPUSHAFTER;
    commandHandlers["BFPUSHAFTER"] = BFPUSHAFTER;
    commandHandlers["FPUSHBEFORE"] = FPUSHBEFORE;
    commandHandlers["BFPUSHBEFORE"] = BFPUSHBEFORE;
    commandHandlers["FDELHEAD"] = FDELHEAD;
    commandHandlers["BFDELHEAD"] = BFDELHEAD;
    commandHandlers["FDELTAIL"] = FDELTAIL;
    commandHandlers["BFDELTAIL"] = BFDELTAIL;
    commandHandlers["FDELAFTER"] = FDELAFTER;
    commandHandlers["BFDELAFTER"] = BFDELAFTER;
    commandHandlers["FDELBEFORE"] = FDELBEFORE;
    commandHandlers["BFDELBEFORE"] = BFDELBEFORE;
    commandHandlers["FDELVALUE"] = FDELVALUE;
    commandHandlers["BFDELVALUE"] = BFDELVALUE;
    commandHandlers["FGET"] = FGET;
    commandHandlers["BFGET"] = BFGET;

    commandHandlers["LCREATE"] = LCREATE;
    commandHandlers["BLCREATE"] = BLCREATE;
    commandHandlers["LPUSHHEAD"] = LPUSHHEAD;
    commandHandlers["BLPUSHHEAD"] = BLPUSHHEAD;
    commandHandlers["LPUSHTAIL"] = LPUSHTAIL;
    commandHandlers["BLPUSHTAIL"] = BLPUSHTAIL;
    commandHandlers["LPUSHAFTER"] = LPUSHAFTER;
    commandHandlers["BLPUSHAFTER"] = BLPUSHAFTER;
    commandHandlers["LPUSHBEFORE"] = LPUSHBEFORE;
    commandHandlers["BLPUSHBEFORE"] = BLPUSHBEFORE;
    commandHandlers["LDELHEAD"] = LDELHEAD;
    commandHandlers["BLDELHEAD"] = BLDELHEAD;
    commandHandlers["LDELTAIL"] = LDELTAIL;
    commandHandlers["BLDELTAIL"] = BLDELTAIL;
    commandHandlers["LDELAFTER"] = LDELAFTER;
    commandHandlers["BLDELAFTER"] = BLDELAFTER;
    commandHandlers["LDELBEFORE"] = LDELBEFORE;
    commandHandlers["BLDELBEFORE"] = BLDELBEFORE;
    commandHandlers["LDELVALUE"] = LDELVALUE;
    commandHandlers["BLDELVALUE"] = BLDELVALUE;
    commandHandlers["LGET"] = LGET;
    commandHandlers["BLGET"] = BLGET;

    commandHandlers["SCREATE"] = SCREATE;
    commandHandlers["BSCREATE"] = BSCREATE;
    commandHandlers["SPUSH"] = SPUSH;
    commandHandlers["BSPUSH"] = BSPUSH;
    commandHandlers["SPOP"] = SPOP;
    commandHandlers["BSPOP"] = BSPOP;

    commandHandlers["QCREATE"] = QCREATE;
    commandHandlers["BQCREATE"] = BQCREATE;
    commandHandlers["QPUSH"] = QPUSH;
    commandHandlers["BQPUSH"] = BQPUSH;
    commandHandlers["QPOP"] = QPOP;
    commandHandlers["BQPOP"] = BQPOP;

    commandHandlers["TCREATE"] = TCREATE;
    commandHandlers["BTCREATE"] = BTCREATE;
    commandHandlers["TINSERT"] = TINSERT;
    commandHandlers["BTINSERT"] = BTINSERT;
    commandHandlers["TDEL"] = TDEL;
    commandHandlers["BTDEL"] = BTDEL;
    commandHandlers["TGET"] = TGET;
    commandHandlers["BTGET"] = BTGET;

    commandHandlers["DCREATE"] = DCREATE;
    commandHandlers["BDCREATE"] = BDCREATE;
    commandHandlers["DINSERT"] = DINSERT;
    commandHandlers["BDINSERT"] = BDINSERT;
    commandHandlers["DSEARCH"] = DSEARCH;
    commandHandlers["BDSEARCH"] = BDSEARCH;
    commandHandlers["DREMOVE"] = DREMOVE;
    commandHandlers["BDREMOVE"] = BDREMOVE;

    commandHandlers["CCREATE"] = CCREATE;
    commandHandlers["BCCREATE"] = BCCREATE;
    commandHandlers["CINSERT"] = CINSERT;
    commandHandlers["BCINSERT"] = BCINSERT;
    commandHandlers["CSEARCH"] = CSEARCH;
    commandHandlers["BCSEARCH"] = BCSEARCH;
    commandHandlers["CREMOVE"] = CREMOVE;
    commandHandlers["BCREMOVE"] = BCREMOVE;

    commandHandlers["PRINT"] = PRINT;
    commandHandlers["BPRINT"] = BPRINT;
    commandHandlers["HELP"] = HELP;
}

void processCommand(const string& command) {
    vector<string> param;
    stringstream sss(command);
    string tok;
    
    while (sss >> tok) {
        param.push_back(tok);
    }
    
    if (param.empty()) {
        return;
    }
    
    string cmd = param[0];
    
    try {
        auto handler = commandHandlers.find(cmd);
        if (handler != commandHandlers.end()) {
            handler->second(param);
        } else {
            cout << "Ошибка: неизвестная команда '" << cmd << "'" << '\n';
            cout << "Введите HELP для просмотра доступных команд" << '\n';
        }
    } catch (const exception& e) {
        cout << e.what() << '\n';
    }
}

auto main(int argc, char* argv[]) -> int {
    vector<string> queries;
    bool binaryMode = false;
    initializeCommands();
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            currFilename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            queries.emplace_back(argv[++i]);
        } else if (arg == "--binary") {
            binaryMode = true;
        }
    }
    
    try {
        if (!currFilename.empty()) {
            if (binaryMode) {
                loadFromBinaryFile();
            } else {
                loadFromFile();
            }
        }
        
        for (const auto& query : queries) {
            processCommand(query);
        }
        
    } catch (const exception& e) {
        cout << e.what() << '\n';
        return 1;
    }
    
    return 0;
}