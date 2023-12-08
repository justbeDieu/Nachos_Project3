#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB {
private:


public:
    PCB(int id);           // Initialize
    ~PCB();          // De-allocate
    int Exec(char* filename, int pID);
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    
}

#endif // PCB_H