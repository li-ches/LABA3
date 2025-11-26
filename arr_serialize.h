#ifndef ARR_SERIALIZE_H
#define ARR_SERIALIZE_H

#include "arr.h"

class ArrSerializer {
public:
    static void saveToFile(const MyArr& arr, const std::string& filename);
    static void loadFromFile(MyArr& arr, const std::string& filename);
    static void saveToBinaryFile(const MyArr& arr, const std::string& filename);
    static void loadFromBinaryFile(MyArr& arr, const std::string& filename);
};

#endif