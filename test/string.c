/*
    A program to test the string functions in the Nachos system.
*/

#include "syscall.h"

int main() {
    char str[256];
    int length = 5;
    //PrintString("Enter string length (<=255): ");
    //ReadString(str, ReadNum());
    //PrintString("You entered: ");
    PrintString("Enter a string: ");
    ReadString(str, length);
    Halt();
}
