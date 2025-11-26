#include "serialize.h"
#include <iostream>

using namespace std;

void writeString(ostream& os, const string& str) {
    size_t size = str.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(str.c_str(), size);
}

string readString(istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (is.fail()) return "";
    
    string str(size, '\0');
    is.read(&str[0], size);
    if (is.fail()) return "";
    return str;
}

void writeStringText(ofstream& file, const string& s) {
    file << s << "\n";
}

string readStringText(ifstream& file) {
    string s;
    getline(file, s);
    return s;
}