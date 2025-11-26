#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>
#include <fstream>

// Общие функции сериализации для строк
void writeString(std::ostream& os, const std::string& str);
std::string readString(std::istream& is);

// Текстовые версии (для удобства)
void writeStringText(std::ofstream& file, const std::string& s);
std::string readStringText(std::ifstream& file);

#endif