#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 


#include "kernel.h"
#include "synchconsole.h"
#include "syscall.h"


void SysReadChar()
{
	char input;
	input=kernel->synchConsoleIn->GetChar();
	if (input=='\n')
		DEBUG(dbgSys, "Character reading failed\n");
	kernel->machine->WriteRegister(2,input);
}

void SysPrintChar()
{
	char result = (char)kernel->machine->ReadRegister(4);
	kernel->synchConsoleOut->PutChar(result);
}
