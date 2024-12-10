#ifndef DIRECTORY_ENTRY_H
#define DIRECTORY_ENTRY_H

#include <vector>

struct DirectoryEntry {
    int state_block;              // State of the block (e.g., INVALID = 0, SHARED = 1, MODIFIED = 2)
    std::vector<bool> sharers;    
};

#endif // DIRECTORY_ENTRY_H
