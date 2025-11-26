#ifndef QUEUE_SERIALIZE_H
#define QUEUE_SERIALIZE_H

#include "queue.h"

class QueueSerializer {
public:
    static void saveToFile(const Queue& queue, const std::string& filename);
    static void loadFromFile(Queue& queue, const std::string& filename);
    static void saveToBinaryFile(const Queue& queue, const std::string& filename);
    static void loadFromBinaryFile(Queue& queue, const std::string& filename);
};

#endif