/*
    A program to test the string functions in the Nachos system.
*/

#include "syscall.h"
int main() {
    char str[256];
    int length;

    PrintString("Enter string length (<=255): ");
    length = ReadInt();
    PrintString("Enter string: ");
    ReadString(str, length);
    PrintString("You entered: ");
    PrintString(str);
    Halt();
}
