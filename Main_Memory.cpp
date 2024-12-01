#include "Main_Memory.h"
#include <stdexcept>
#include <bits/stdc++.h>
using namespace std;

Main_Memory::Main_Memory(int memory_size, int block_size) {
    if (memory_size <= 0 || block_size <= 0 || memory_size % block_size != 0) {
        throw std::invalid_argument("Invalid memory or block size");
    }
    this->memory_size = memory_size;
    this->block_size = block_size;
    total_memory.assign(memory_size / block_size, {std::vector<int>(block_size, 0), 0, "x"});
}

Cache_Block Main_Memory::read_memory(const std::string& address) {
    int address_pos = std::stoi(address, nullptr, 2);
    int offset_size = std::log2(block_size);
    address_pos >>= offset_size;
    if (address_pos < 0 || address_pos >= total_memory.size()) {
        throw std::out_of_range("Address out of range");
    }
    return total_memory[address_pos];
}

void Main_Memory::write_memory(const std::string& address, const Cache_Block& new_block) {
    int address_pos = std::stoi(address, nullptr, 2);
    int offset_size = std::log2(block_size);
    address_pos >>= offset_size;

    if (address_pos < 0 || address_pos >= total_memory.size()) {
        throw std::out_of_range("Address out of range");
    }
    total_memory[address_pos] = new_block;
}
