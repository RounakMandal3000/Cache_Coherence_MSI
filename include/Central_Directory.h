#ifndef CENTRAL_DIRECTORY_H
#define CENTRAL_DIRECTORY_H

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include "Processor.h"
#include "Main_Memory.h"
#include "Directory_Entry.h"
using namespace std;

class Processor;

class Central_Directory {
public:
    std::map<const std::string, DirectoryEntry> cen_directory;
    std::map<int, Processor*> directory; // Maps block addresses to directory entries
    int num_processors; 
    int block_size;
    int cache_size;
    Central_Directory(int num_processors, const unsigned int block_size, const unsigned int cache_size);
    void init_procs(int num_processors, Processor* processors[]);
    void invalidate(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id);
    Cache_Block read_request(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id, Main_Memory* M);
    Cache_Block write_request(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id, Main_Memory* M);
};

#endif // CENTRAL_DIRECTORY_H
