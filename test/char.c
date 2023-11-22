#include "syscall.h" 
#include "copyright.h" 

int main() 
{ 

 /*Read and print an char*/ 
    char result = ReadChar();
    PrintChar(result);
    Halt(); 
} 
