#include "syscall.h"

int main()
{
      //read and write an int
      int i = ReadInt();
      PrintInt(i);
      Halt();
}
