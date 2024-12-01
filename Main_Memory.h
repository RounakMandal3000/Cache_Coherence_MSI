#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

#include <vector>
#include <string>
#include <cmath>
#include "Cache_Block.h" 

class Main_Memory {
public:
    std::vector<Cache_Block> total_memory; // Vector representing memory blocks
    int memory_size;                       // Total memory size in bytes
    int block_size;                        // Size of each block in bytes
    Main_Memory(int memory_size, int block_size);
    Cache_Block read_memory(const std::string& address);
    void write_memory(const std::string& address, const Cache_Block& new_block);
};

#endif // MAIN_MEMORY_H
