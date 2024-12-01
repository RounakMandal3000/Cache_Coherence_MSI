#ifndef CACHE_BLOCK_H
#define CACHE_BLOCK_H

#include <vector>
#include <string>

class Cache_Block {
public:
    std::vector<int> data;
    // State of the cache block
    // 0 - Invalid
    // 1 - Shared
    // 2 - Modified/Exclusive
    int state;
    std::string tag;
};

#endif // CACHE_BLOCK_H
