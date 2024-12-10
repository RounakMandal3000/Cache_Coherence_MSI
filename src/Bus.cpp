#include "Bus.h"
#include "Cache.h" // Make sure this includes your Cache class header
#include "Main_Memory.h" // Include Main_Memory header for memory operations
#include "AddressParser.h"
#include <bits/stdc++.h>
using namespace std;


// Constructor
Bus::Bus(std::vector<Cache*> caches, Main_Memory* M) {
    this->caches = caches;
    this->M = M;
}

// Method to read from the bus (broadcast read)
Cache_Block Bus::read_broadcast(const std::string& address) {
    int index;
    std::string tag;
    address_data ad = address_info(address, caches[0]->block_size, caches[0]->cache_size);
    // offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    Cache_Block found_cache;
    int c = 0;

    // Iterate through all caches to perform read operation
    for (size_t i = 0; i < caches.size(); i++) {
        int read1 = caches[i]->read(address);
        if (read1 != -1) {
            if (caches[i]->c_blocks[index].state == 0)
                continue;
            c++;
            caches[i]->c_blocks[index].state = 1;  // Mark state as shared
            found_cache = caches[i]->c_blocks[index];
        }
    }
    // cout<<"Read broadcast at address "<<address<<endl;

    // If no cache has the data, read it from the main memory
    if (c == 0) {
        return M->read_memory(address);
    } else {
        // cout<<"Found read data at some processor for "<<address<<" !"<<endl;
        return found_cache;
    }
}

// Method to write to the bus (broadcast write)
Cache_Block Bus::write_broadcast(const std::string& address, int data) {
    int index;
    std::string tag;
    address_data ad = address_info(address, caches[0]->block_size, caches[0]->cache_size);
    // offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    Cache_Block found_cache;
    int c = 0;
    // cout<<"Write broadcast at address "<<address<<endl;

    // Iterate through all caches to perform write operation
    for (size_t i = 0; i < caches.size(); i++) {
        int read1 = caches[i]->read(address);
        if (read1 != -1) {
            if (caches[i]->c_blocks[index].state == 0)
                continue;
            c++;
            caches[i]->c_blocks[index].state = 0;  // Mark as invalidated
            found_cache = caches[i]->c_blocks[index];
        }
    }
    // cout<<"Write broadcast at address "<<address<<endl;

    // If no cache has the data, perform the read from main memory
    if (c == 0) {
        return M->read_memory(address);
    } else {
        // cout<<"Found write data at some processor for "<<address<<" !"<<endl;
        return found_cache;
    }
}

// Method to invalidate cache blocks for a given address
void Bus::invalidate(const std::string& address) {
    int index;
    std::string tag;
    address_data ad = address_info(address, caches[0]->block_size, caches[0]->cache_size);
    // offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    // Iterate through all caches and invalidate the cache block with matching tag
    for (size_t i = 0; i < caches.size(); i++) {
        if (caches[i]->c_blocks[index].tag == tag) {
            caches[i]->c_blocks[index].state = 0;  // Set state to invalid
        }
    }
}

