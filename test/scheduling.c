#include "syscall.h"

void main()
{
    int PingPID, PongPID;
    PrintString("PingPong test starting...\n");
    
    PingPID = Exec("./test/ping");
    PongPID = Exec("./test/pong");
    Join(PingPID);
    Join(PongPID);
}
