#include "Processor.h"
#include "Bus.h"            // Include Bus class header
#include "Cache.h"          // Include Cache class header
#include "Main_Memory.h"    // Include Main_Memory class header
#include "AddressParser.h"
#include "Central_Directory.h"
#include <bits/stdc++.h>
using namespace std;

Processor::Processor(const unsigned int cache_size, const unsigned int block_size, int id, Main_Memory* M, int num_processors, Central_Directory* CEN) {
    cout<<"S"<<endl;
    cache = new Cache(cache_size, block_size, id, M);
    this->id = id;
    this->CEN = CEN;
    this->M = M;
}

int Processor::read(const std::string& address, Bus* bus, Central_Directory* CEN, int mode) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, cache->block_size, cache->cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;
    int read1 = cache->read(address);
    if (read1 == -1) {
        Cache_Block cache1;
        if(mode==0)
            cache1 = CEN->read_request(address, cache->block_size, cache->cache_size, id, M);
        else
            cache1 = bus->read_broadcast(address);
        
        read1 = cache1.data[offset];  // Read again after replacement
        cache1.state = 1; // Set cache block state to Shared
        if(cache1.tag=="x")
            cache1.tag = tag;
        cache->replace(index, address, cache1); // Replace the cache block with the fetched block
        return read1;
    } else {
        // Cache hit
        // cout<<"Cache Hit Read from Processor "<<id<<" at address "<<address<<" "<<cache->c_blocks[index].data[offset]<<endl;
        return -read1;
    }
}

// Method to write to the processor cache, using bus if needed
int Processor::write(const string& address, int data, Bus* bus, Central_Directory* CEN, int mode) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, cache->block_size, cache->cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    // Try to write to cache
    int write1 = cache->write(address, data);
    if (write1 == 0) {
        // If cache hit and state is not Modified, invalidate the block in other caches
        if (cache->c_blocks[index].state != 2){
            if(mode==0)
                CEN->invalidate(address, cache->block_size, cache->cache_size, id);
            else
                bus->invalidate(address);
        }
        cache->c_blocks[index].state = 2;  // Mark the block as Modified
        return 1;
        // cout<<"Cache Hit Write from Processor "<<id<<" at address "<<address<<" "<<cache->c_blocks[index].data[offset]<<endl;
    } else {
        // If cache miss, broadcast write to other caches or main memory
        // cout<<"Cache Miss Write from Processor "<<id<<" at address "<<address<<endl;
         Cache_Block cache1;
        if(mode==0)
            cache1 = CEN->write_request(address, cache->block_size, cache->cache_size, id, M);
        else{
            cache1 = bus->write_broadcast(address, data);
            bus->invalidate(address);  // Invalidate other caches
        }
        cache1.state = 2;
        cache1.data[offset] = data;
        if(cache1.tag=="x")
            cache1.tag = tag;
        cache->replace(index, address, cache1);  // Replace the cache block
        return 0;
    }
}


