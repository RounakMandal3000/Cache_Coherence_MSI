#ifndef ADDRESSPARSER_H
#define ADDRESSPARSER_H

#include <string>
#include <cmath>
#include "AddressData.h"


inline address_data address_info(const std::string& address, const unsigned int block_size, const unsigned int cache_size) {
    unsigned int offset, index;
    std::string tag;
    
    int offset_size = std::log2(block_size);  // Number of bits for the offset
    int index_size = std::log2(cache_size / block_size);  // Number of bits for the index
    
    offset = std::stoi(address.substr(address.length() - offset_size, offset_size), nullptr, 2);
    index = std::stoi(address.substr(address.length() - offset_size - index_size, index_size), nullptr, 2);
    tag = address.substr(0, address.length() - offset_size - index_size);

    address_data ad = {offset, index, tag};
    return ad;
}

#endif // ADDRESSPARSER_H
