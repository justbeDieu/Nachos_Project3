#include "syscall.h"

void main()
{
    int PingPID, PongPID;
    PrintString("PingPong Testing: \n");
    
    PingPID = Exec("./test/ping");
    PongPID = Exec("./test/pong");
    Join(PingPID);
    Join(PongPID);
}
