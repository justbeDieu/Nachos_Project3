#ifndef PTABLE.H
#define PTABLE.H

#include "bitmap.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable {
private:
    

public: 
    PTable(int size);           // Initialize
    ~PTable();          // De-allocate
    int ExecUpdate(char* filename); 
    int ExitUpdate(int ec); 
    int JoinUpdate(int pID); 
    int GetFreeSlot(); 
    void Remove(int pID);
}

#endif // PTABLE.H