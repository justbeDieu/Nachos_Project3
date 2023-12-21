#include "syscall.h"

int main()
{
    int PingPID, PongPID;
    PrintString("PingPong Test: \n");
    
    PingPID = Exec("./test/ping");
    PongPID = Exec("./test/pong");
    Join(PingPID);
    Join(PongPID);

    Halt();
}