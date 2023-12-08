#ifndef PTABLE.H
#define PTABLE.H

#include "bitmap.h"
#include "pcb.h"
#include "schandle.h"
#include "semaphore.h"

#define MAX_PROCESS 10

class PTable {
private:
    BitMap *bm;
    PCB *pcbTab[MAX_PROCESS];
    int psize;
    Semaphore *bmsem;

public: 
    PTable(int size);           // Initialize
    ~PTable();          // De-allocate
    int ExecUpdate(char* filename); 
    int ExitUpdate(int ec); 
    int JoinUpdate(int pID); 
    int GetFreeSlot(); 
    bool IsExist(int pID);
    void Remove(int pID);
}

#endif // PTABLE.H