#ifndef BUS_H
#define BUS_H

#include <vector>
#include <string>
#include "Main_Memory.h"  // Include the Main_Memory header
#include "Cache.h"        // Include the Cache header (you should have a Cache class)

class Bus {
public:
    Main_Memory* M;                // Pointer to main memory
    std::vector<Cache*> caches;     // List of caches connected to the bus
    Bus(std::vector<Cache*> caches, Main_Memory* M);
    Cache_Block read_broadcast(const std::string& address);
    Cache_Block write_broadcast(const std::string& address, int data);
    void invalidate(const std::string& address);

};

#endif // BUS_H
