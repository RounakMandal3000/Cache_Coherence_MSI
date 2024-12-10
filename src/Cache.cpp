#include "Cache.h"
#include "AddressParser.h"
#include <bits/stdc++.h>
using namespace std;


Cache::Cache(unsigned int cache_size, unsigned int block_size, int id, Main_Memory* M) {
    this->c_blocks.assign(cache_size / block_size, {std::vector<int>(block_size, 0), 0, "x"});
    this->cache_size = cache_size;
    this->block_size = block_size;
    this->id = id;
    this->M = M;
}

int Cache::read(const std::string& address) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;
    Cache_Block found = c_blocks[index];
    if (found.tag == tag && found.state!=0) {
        return found.data[offset];
    }
    return -1; // Cache miss: return -1 and read from main memory
}

int Cache::write(const std::string& address, int data_input) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    Cache_Block& found = c_blocks[index];
    if (found.tag == tag) {
        found.data[offset] = data_input;
        return 0; // Successful write
    }
    return -1; // Cache miss
}

void Cache::writeBack(Cache_Block& block, const std::string& address) {
    M->write_memory(address, block);
}

void Cache::replace(int index, const std::string& address, Cache_Block& new_cache_block) {
    if (c_blocks[index].state == 2) {
        writeBack(c_blocks[index], address); // Write-back only when modified
    }
    c_blocks[index] = new_cache_block;
}

