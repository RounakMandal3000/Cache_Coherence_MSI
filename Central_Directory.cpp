#include <bits/stdc++.h>
using namespace std;
#include "Central_Directory.h"
#include "AddressParser.h"
#include "Main_Memory.h"



Central_Directory::Central_Directory(int num_processors, const unsigned int block_size, const unsigned int cache_size){
	this->num_processors = num_processors; 
	this->block_size = block_size;  
	this->cache_size = cache_size; 
}
void Central_Directory::init_procs(int num_processors, Processor* processors[]){
	for(int i=0;i<num_processors; i++)
		directory[i] = processors[i];
}

Cache_Block Central_Directory::read_request(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id, Main_Memory* M){
	int offset, index, proc_id;
    std::string tag;
	address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;
	if(cen_directory.find(address)!=cen_directory.end() && cen_directory[address].state_block != 0){
		cen_directory[address].state_block = 1;
		Processor* reply_proc;
		for(int i=0;i<num_processors;i++){
			if(cen_directory[address].sharers[i] == false)
				continue;
			reply_proc = directory[i];

			cen_directory[address].sharers[i] = true;
			directory[i]->cache->c_blocks[index].state = 1;

		}

		cen_directory[address].sharers[calling_proc_id] = true;
		return reply_proc->cache->c_blocks[index];
	}
	else{
		if(cen_directory[address].sharers.size() == 0){
			cen_directory[address].sharers.assign(num_processors, false);
		}
		cen_directory[address].state_block = 1;

		cen_directory[address].sharers[calling_proc_id] = true;
		return M->read_memory(address);
	}

}

Cache_Block Central_Directory::write_request(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id, Main_Memory* M){
	int offset, index, proc_id;
    std::string tag;
	address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

	if(cen_directory.find(address)!=cen_directory.end() && cen_directory[address].state_block != 0){
		cen_directory[address].state_block = 2;
		Processor* reply_proc;
		for(int i=0;i<num_processors;i++){
			if(cen_directory[address].sharers[i] == false)
				continue;
			reply_proc = directory[i];
			cen_directory[address].sharers[i] = false;
			reply_proc->cache->c_blocks[index].state = 0;
		}
		
		cen_directory[address].sharers[calling_proc_id] = true;
		return reply_proc->cache->c_blocks[index];
	}
	else{
		cen_directory[address].state_block = 2;
		if(cen_directory[address].sharers.size() == 0){
			cen_directory[address].sharers.assign(num_processors, false);
		}
		cen_directory[address].sharers[calling_proc_id] = true;
		return M->read_memory(address);
	}
}

void Central_Directory::invalidate(const std::string& address, const unsigned int block_size, const unsigned int cache_size, int calling_proc_id){
	int offset, index, proc_id;
    std::string tag;
	address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;
	if(cen_directory.find(address)!=cen_directory.end() && cen_directory[address].state_block != 0){
		cen_directory[address].state_block = 2;
		for(int i=0;i<num_processors;i++){
			if(cen_directory[address].sharers[i] == false)
				continue;
			cen_directory[address].sharers[i] = false;
			directory[i]->cache->c_blocks[index].state = 0;
		}
		cen_directory[address].sharers[calling_proc_id] = true;
	}
}