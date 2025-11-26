#ifndef LIST_SERIALIZE_H
#define LIST_SERIALIZE_H

#include "list.h"

class ListSerializer {
public:
    static void saveToFile(const MyList& list, const std::string& filename);
    static void loadFromFile(MyList& list, const std::string& filename);
    static void saveToBinaryFile(const MyList& list, const std::string& filename);
    static void loadFromBinaryFile(MyList& list, const std::string& filename);
};

#endif