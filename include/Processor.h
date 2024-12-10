#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include "Cache.h"         
#include "Main_Memory.h"   
#include "Bus.h"           
#include "Central_Directory.h"

class Central_Directory;

class Processor {
public:
    Cache* cache;              
    int id;                    
    Main_Memory* M;            
    Central_Directory* CEN;
    Processor(const unsigned int cache_size, const unsigned int block_size, int id, Main_Memory* M,  int num_processor, Central_Directory* CEN);
    int read(const std::string& address, Bus* bus, Central_Directory* CEN, int mode);
    int write(const std::string& address, int data, Bus* bus, Central_Directory* CEN, int mode);

};

#endif // PROCESSOR_H
