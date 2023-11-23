#include "syscall.h"

int main() 
{ 
      //read and write an int
      int i;
      PrintString("Enter an integer: ");
      i = ReadInt();
      PrintString("You entered: ");
      PrintInt(i);
      Halt(); 
} 
