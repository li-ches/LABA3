#ifndef DLIST_SERIALIZE_H
#define DLIST_SERIALIZE_H

#include "dlist.h"

class DListSerializer {
public:
    static void saveToFile(const DList& list, const std::string& filename);
    static void loadFromFile(DList& list, const std::string& filename);
    static void saveToBinaryFile(const DList& list, const std::string& filename);
    static void loadFromBinaryFile(DList& list, const std::string& filename);
};

#endif