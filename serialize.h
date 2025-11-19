#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>
#include <fstream>

using namespace std; 

//inline - функция будет включена в несколько .cpp файлов
inline void writeString(ofstream& file, const string& s) {
    size_t len = s.length();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(s.c_str(), len);
}

inline string readString(ifstream& file) {
    size_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (file.fail()) return "";
    
    string s(len, '\0');
    file.read(&s[0], len);
    
    // ВАЖНОЕ ИСПРАВЛЕНИЕ: Проверяем, удалось ли считать данные
    if (file.fail()) return ""; 
    
    return s;
}

#endif