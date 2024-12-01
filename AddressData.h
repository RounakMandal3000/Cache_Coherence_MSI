// AddressData.h
#ifndef ADDRESS_DATA_H
#define ADDRESS_DATA_H

#include <string>

struct address_data {
    unsigned int offset;  
    unsigned int index;   
    std::string tag;     
    
    address_data(unsigned int off = 0, unsigned int ind = 0, std::string t = "", int proc_id = -1)
        : offset(off), index(ind), tag(t){}
};

#endif // ADDRESS_DATA_H
