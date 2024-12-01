#include <bits/stdc++.h>
#include "Processor.h"
#include "Main_memory.h"
#include "Cache_Block.h"
#include "cache.h"
#include "Bus.h"
#include "Central_Directory.h"
#include "AddressData.h"
#include "AddressParser.h"
using namespace std;


int main() {
    const unsigned int block_size = 4;
    const unsigned int total_mem_size = 65536; // 16 -> offset = 2, index = 10, tag = 4
    const unsigned int cache_size = 8192;  // change this

    Main_Memory M(total_mem_size, block_size);
    int num_proc = 4;
    vector<Processor> P;

    int mode = 1; // 0 for directory, 1 for snoop

    Central_Directory CEN(num_proc, block_size, cache_size);
    for(int i=0;i<num_proc;i++){
        Processor p(cache_size, block_size, i, &M, num_proc, &CEN);
        P.push_back(p);
    }
    
    vector<Cache*> caches(num_proc);
    for(int i=0;i<num_proc;i++){
        caches[i] = P[i].cache;
        CEN.directory[i] = &P[i];
    }
    Bus B(caches, &M);
    
    

   int num_operations = 1000000; // Number of operations to perform
    vector<int> addresses;        // Addresses to access
    vector<int> data_inputs;      // Random data for write operations

    // Generate memory addresses and data with locality of reference
    srand(time(0));
    int locality_window = 100;     // Window size for temporal locality
    int num_blocks = total_mem_size / block_size;

    for (int i = 0; i < num_operations;) { // Increment `i` manually within the loop
        // Choose a random block for spatial locality
        int block_base = (rand() % num_blocks) * block_size;

        // Pick an address within the block
        int address = block_base + (rand() % block_size);

        // Insert the first address in a new locality window
        addresses.push_back(address);
        data_inputs.push_back(rand() % 256);
        i++;

        // Add temporal locality by reusing the recent address
        for (int j = 1; j < locality_window && i < num_operations; j++, i++) {
            addresses.push_back(address); // Reuse the same address for locality
            data_inputs.push_back(rand() % 256);
        }
    }

    int hits = 0, misses = 0; // Track cache hits and misses
    clock_t start = clock();

    for (int i = 0; i < num_operations; i++) {
        int proc_id = i % num_proc; // Round-robin processor selection

        Processor& proc = P[proc_id];
        string address = bitset<16>(addresses[i]).to_string(); // Convert address to binary string

        // Randomly decide to read or write
        if (rand() % 2 == 0) { // Read operation
            int result = proc.read(address, &B, &CEN, mode);

            if (result <= 0) { // Cache hit
                hits++;
            } else {           // Cache miss
                misses++;
            }
        } else { // Write operation
            int data = data_inputs[i];
            int result = proc.write(address, data, &B, &CEN, mode);
            if (result == 1) { // Cache hit
                hits++;
            } else {           // Cache miss
                misses++;
            }
        }
    }

    clock_t end = clock();

    // Results
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "Benchmark Results with Locality of Reference:" << endl;
    cout << "Number of Processors: " << num_proc << endl;
    cout << "Number of Operations: " << num_operations << endl;
    cout << "Cache Hits: " << hits << endl;
    cout << "Cache Misses: " << misses << endl;
    cout << "Hit Rate: " << (double(hits) / num_operations) * 100 << "%" << endl;
    cout << "Miss Rate: " << (double(misses) / num_operations) * 100 << "%" << endl;
    cout << "Total Time: " << elapsed_time << " seconds" << endl;


    return 0;
}



