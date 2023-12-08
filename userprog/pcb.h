#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB {
private:
    Semaphore *joinsem;
    Semaphore *exitsem;
    Semaphore *mutex;
    int exitcode;
    Thread *thread;
    int pid;
    int numwait;

public:
    int parentID;
    PCB(int id);           // Initialize
    ~PCB();          // De-allocate
    int Exec(char* filename, int pID);
    int GetNumWait();
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    void SetExitCode(int ec);
    int GetExitCode();
}

#endif // PCB_H