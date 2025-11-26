#ifndef HASH_SERIALIZER_H
#define HASH_SERIALIZER_H

#include "hash.h"

class HashSerializer {
public:
    static void saveToFile(const ChainHash& table, const std::string& filename);
    static void loadFromFile(ChainHash& table, const std::string& filename);
    static void saveToBinaryFile(const ChainHash& table, const std::string& filename);
    static void loadFromBinaryFile(ChainHash& table, const std::string& filename);

    static void saveToFile(const OpenHash& table, const std::string& filename);
    static void loadFromFile(OpenHash& table, const std::string& filename);
    static void saveToBinaryFile(const OpenHash& table, const std::string& filename);
    static void loadFromBinaryFile(OpenHash& table, const std::string& filename);
};

#endif