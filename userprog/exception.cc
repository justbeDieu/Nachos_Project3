// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include <string>
#include <string.h>
#include "synchconsole.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
void IncreasePC() 
{
    int counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter + 4);
} 
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch(which)
    {
	case PageFaultException:
	{
		DEBUG('a', "No valid translation found.\n");
		printf("No valid translation found.\n");
		interrupt->Halt();
		break;
	}
	case ReadOnlyException:
	{
		DEBUG('a', "Write attempted to page marked read-only.\n");
		printf("Write attempted to page marked 'read-only'.\n");
		interrupt->Halt();
		break;	
	}
	case BusErrorException:
	{
		DEBUG('a', "Translation resulted in an invalid physical address\n");
		printf("Translation resulted in an invalid physical address.\n");
		interrupt->Halt();
		break;
	}
	case AddressErrorException:
	{
		DEBUG('a', "Unaligned reference or one that was beyond the end of the address space.\n");
		printf("Unaligned reference or one that was beyond the end of the address space.\n");
		interrupt->Halt();
		break;
	}
	case OverflowException:
	{
		DEBUG('a', "Integer overflow in add or sub.\n");
		printf("Integer overflow in add or sub.\n");
		interrupt->Halt();
		break;
	}
	case IllegalInstrException:
	{
		DEBUG('a', "Unimplemented or reserved instr.\n");
		printf("%d %d occurs\n", which, type);
		interrupt->Halt();
		break;	
	}
	case NumExceptionTypes:
	{
		DEBUG('a', "NumExceptionTypes.\n");
		printf("NumExceptionTypes.\n");
		interrupt->Halt();
		break;
	}
	case NoException:
	{
	return;
	}
   	case SyscallException:
	{
		switch(type)
		{
			case SC_Exit:
			{
   	        	Exit(0);
            	break;
			}
			case SC_Halt: 
			{
			    DEBUG('a', "Shutdown, initiated by user program.\n");
	   	        interrupt->Halt();
		        break;
        	}
			case SC_ReadInt: 
			{
				char* buffer;
			int MAX_BUFFER = 255;
			buffer = new char[MAX_BUFFER + 1];
			int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);
			int number = 0;
			
			           
			bool isNegative = false; 
			int firstNumIndex = 0;
			if (buffer[0] == '-')
			{
				isNegative = true;
				firstNumIndex = 1;
			}

			
			for (int i = firstNumIndex; i < numbytes; i++)
			{
				if (buffer[i] < '0' || buffer[i] > '9')
				{
					printf("\n The integer number is not valid");
					DEBUG('a', "\n The integer number is not valid");
					machine->WriteRegister(2, number);
					IncreasePC();
					delete buffer;
					return;
				}
			}

			if (numbytes > 11 || (numbytes == 11 && !isNegative))
			{
				printf("\n The number is overvalued int");
				DEBUG('a', "\n The number is overvalued int");
				machine->WriteRegister(2, number);
				IncreasePC();
				delete buffer;
				return;
			}
			int checkMin[] = { 2, 1, 4, 7, 4, 8, 3, 6, 4, 8 };
			
			if(numbytes == 11)
			{
				
				for (int i = firstNumIndex; i < numbytes; i++)
				{
					int temp = (int)(buffer[i] - 48);
					if (temp == checkMin[i - 1]) {
						continue;
					}
					else if (temp > checkMin[i - 1]) {
						printf("\n\n The number is overvalued int");
						DEBUG('a', "\n The number is overvalued int");
						machine->WriteRegister(2, number);
						IncreasePC();
						delete buffer;
						return;
					}
					else
						break;
				}
			}
			
			int checkMax[] = { 2, 1, 4, 7, 4, 8, 3, 6, 4, 7 };
			if (numbytes == 10 && !isNegative)	
			{
				for (int i = firstNumIndex; i < numbytes; i++)
				{
					int temp = (int)(buffer[i] - 48);
					if (temp == checkMax[i]) {
						continue;
					}
					else if (temp > checkMax[i]) {
						printf("\n\n The number is overvalued int");
						DEBUG('a', "\n The number is overvalued int");
						machine->WriteRegister(2, number);
						IncreasePC();
						delete buffer;
						return;
					}
					else
						break;
				}
			}

			
			for (int i = firstNumIndex; i < numbytes; i++)
			{
				number = number * 10 + (int)(buffer[i] - 48);
			}

			if (isNegative)
			{
				number = number * -1;
			}
			machine->WriteRegister(2, number);
			IncreasePC();
			delete buffer;
			return;
			}
			case SC_PrintInt: 
			{
			    int number = machine->ReadRegister(4);
			if(number == -2147483648)
			{
				gSynchConsole->Write("-2147483648", 11);
				IncreasePC();
				return;
			}
			if (number == 0)
			{
				gSynchConsole->Write("0", 1); 
				IncreasePC();
				return;
			}

			
			bool isNegative = false; 
			int N = 0; 
			int firstNumIndex = 0;

			if (number < 0)
			{
				isNegative = true;
				number = number * -1;
				firstNumIndex = 1;
			}

			int temp = number; 
			while (temp > 0)
			{
				N++;
				temp /= 10;
			}

			
			char* buffer;
			int MAX_BUFFER = 10;
			if (isNegative)
				MAX_BUFFER = 11;
			buffer = new char[MAX_BUFFER];
			
			for (int i = firstNumIndex + N - 1; i >= firstNumIndex; i--)
			{
				buffer[i] = (char)((number % 10) + 48);
				number /= 10;
			}
			if (isNegative)
			{
				buffer[0] = '-';
				N++;
			}
			gSynchConsole->Write(buffer, N);
			delete buffer;
			IncreasePC();
			return; 
			}
			case SC_ReadChar:
			{
				char input;
				do
					input=kernel->synchConsoleIn->GetChar();
				while (input=='\n')
				kernel->machine->WriteRegister(2,input);
				IncreasePC(); 
				break;
			}
			case SC_PrintChar:
			{
				char c = static_cast<char>(machine->ReadRegister(4)); 
				kernel->synchConsoleOut->PutChar(c);
				IncreasePC(); 
				break;
			}
			case SC_ReadString:
			{
				int addr = machine->ReadRegister(4); 
				int length = machine->ReadRegister(5); 

				char* buffer = machine->User2System(addr, length); 
				gSynchConsole->Read(buffer, length); 
				machine->System2User(addr, length, buffer); 
				delete buffer; 
				IncreasePC(); 
				break;
			}
			case SC_PrintString:
			{
				int addr = machine->ReadRegister(4); 
				char* buffer = machine->User2System(addr, 255); 
				int length = 0;
			
				while (buffer[length] != 0) length++; 
				gSynchConsole->Write(buffer, length + 1); 

				delete buffer;
				IncreasePC(); 
				break;
			}
			default:
			{ 
				
				IncreasePC();
				break;
			}
        }       
		break;
	}
	default:
	{
		IncreasePC();
		break;
	}
	}
}
