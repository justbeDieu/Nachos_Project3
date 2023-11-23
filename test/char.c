#include "syscall.h" 
#include "copyright.h" 

int main() 
{ 
 /*Read and print a char*/ 
    char result;
    PrintString("Enter a character: ");
    result = ReadChar();
    PrintString("You entered: ");
    PrintChar(result);
    Halt(); 
} 
