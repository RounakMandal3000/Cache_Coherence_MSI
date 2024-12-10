#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <string>
#include "Main_Memory.h"  // Include Main_Memory header
#include "Cache_Block.h"  // Include Cache_Block header
#include "AddressData.h"

class Cache {
public:
    Main_Memory* M;                // Pointer to the main memory
    std::vector<Cache_Block> c_blocks;  // Cache blocks
    int cache_size;                 // Size of the cache in bytes
    int block_size;                 // Size of each block in bytes
    int id;                         // Cache ID (for identification, e.g., processor ID)
    Cache(unsigned int cache_size, unsigned int block_size, int id, Main_Memory* M);
    int read(const std::string& address);
    int write(const std::string& address, int data_input);
    void writeBack(Cache_Block& block, const std::string& address);
    void replace(int index, const std::string& address, Cache_Block& new_cache_block);
};

#endif // CACHE_H
