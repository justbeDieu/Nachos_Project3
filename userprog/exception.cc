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
#define MAX_INT32_LENGTH 512 
#define MAX_BUFFER_SIZE	512

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
			case SC_Halt: 
			{
			    DEBUG('a', "Shutdown, initiated by user program.\n");
		  	 	interrupt->Halt();
			    break;
	        }
			case SC_Exec:
			{
				int virtualAddr;
				char* programName;
				virtualAddr = machine->ReadRegister(4);
				programName = machine->User2System(virtualAddr, MAX_BUFFER_SIZE);
				OpenFile *file = fileSystem->Open(programName);	
				if(programName == NULL)
				{
					DEBUG('a',"Not enough memory\n");
					printf("Not enough memory\n");
					machine->WriteRegister(2,-1);
					break;
				}
				if(file == NULL)
				{
					DEBUG('a',"Cannot open file\n");
					printf("Cannot open file\n");
					machine->WriteRegister(2,-1);
					break;
				}
				int id = pTab->ExecUpdate(programName);
				machine->WriteRegister(2,id);
				delete[]programName;
				delete file;
				IncreasePC();
				break;
			}
			case SC_Join:
			{
				int id, exitCode;
				id = machine->ReadRegister(4);
				exitCode = pTab->JoinUpdate(id);
				machine->WriteRegister(2, exitCode);
				IncreasePC();
				break;
			}
			case SC_Exit:
			{
				int status = machine->ReadRegister(4);
				if(status)
					break;
				int exitCode = pTab->ExitUpdate(status);
				machine->WriteRegister(2, exitCode);
				if (currentThread->space!=NULL)
					delete currentThread->space;
				currentThread->Finish();
				IncreasePC();
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
				char* input = new char;
				do
					gSynchConsole->Read(input,1);
				while ((*input)=='\n');
				machine->WriteRegister(2,*input);
				IncreasePC(); 
				break;
			}
			case SC_PrintChar:
			{
				char c = static_cast<char>(machine->ReadRegister(4)); 
				gSynchConsole->Write(&c,1);
				IncreasePC(); 
				break;
			}
			case SC_ReadString:
			{
				int addr = machine->ReadRegister(4); 
				int length = machine->ReadRegister(5); 

				char* buffer = new char[MAX_BUFFER_SIZE]; 
				int num = gSynchConsole->Read(buffer, length); 
				buffer[num] = 0;
				machine->System2User(addr, num + 1, buffer);
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
			case SC_CreateFile:
			{
				int virtAddr;
				char* filename;
				DEBUG('a', "\n SC_CreateFile call ...");
				DEBUG('a', "\n Reading virtual address of filename");
				//Read address of register 4
				virtAddr = machine->ReadRegister(4); 
				DEBUG('a', "\n Reading filename.");
				filename = machine->User2System(virtAddr, MaxFileLength + 1);
				if (strlen(filename) == 0)
				{
					printf("\n File name is not valid");
					DEBUG('a', "\n File name is not valid");
					machine->WriteRegister(2, -1); 
					IncreasePC();
					break;
				}
				
				if (filename == NULL)  
				{
					printf("\n Not enough memory in system");
					DEBUG('a', "\n Not enough memory in system");
					machine->WriteRegister(2, -1); 
					delete filename;
					IncreasePC();
					break;
				}
				DEBUG('a', "\n Finish reading filename.");
				
				if (!fileSystem->Create(filename, 0)) 
				{
					printf("\n Error create file '%s'", filename);
					machine->WriteRegister(2, -1);
					delete filename;
					IncreasePC();
					break;
				}
				machine->WriteRegister(2, 0);
				delete filename;
				IncreasePC(); 
				break;
			}

			case SC_Open:
			{
				int virtAddr = machine->ReadRegister(4); 
				int type = machine->ReadRegister(5); 
				char* filename;
				filename = machine->User2System(virtAddr, MaxFileLength); 
				int freeSlot = fileSystem->FindFreeSlot();
				if (freeSlot != -1) 
				{
					if (type == 0 || type == 1) 
					{
						
						if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) 
						{
							machine->WriteRegister(2, freeSlot); 
						}
					}
					else if (type == 2) 
					{
						machine->WriteRegister(2, 0); 
					}
					else 
					{
						machine->WriteRegister(2, 1); 
					}
					delete[] filename;
					break;
				}
				machine->WriteRegister(2, -1); 
				
				delete[] filename;
				break;
			}

			case SC_Close:
			{
				int fid = machine->ReadRegister(4); 
				if (fid >= 0 && fid <= 14) 
				{
					if (fileSystem->openf[fid]) 
					{
						delete fileSystem->openf[fid]; 
						fileSystem->openf[fid] = NULL; 
						machine->WriteRegister(2, 0);
						break;
					}
				}
				machine->WriteRegister(2, -1);
				break;
			}

			case SC_Read:
			{
				int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
				int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
				int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
				int OldPos;
				int NewPos;
				char *buf;
				if (id < 0 || id > 14)
				{
					printf("\nKhong the read vi id nam ngoai bang mo ta file.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				if (fileSystem->openf[id] == NULL)
				{
					printf("\nKhong the read vi file nay khong ton tai.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				if (fileSystem->openf[id]->type == 3) 
				{
					printf("\nKhong the read file stdout.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				OldPos = fileSystem->openf[id]->GetCurrentPos(); 
				buf = machine->User2System(virtAddr, charcount); 
				
				if (fileSystem->openf[id]->type == 2)
				{`
					int size = gSynchConsole->Read(buf, charcount); 
					machine->System2User(virtAddr, size, buf); 
					machine->WriteRegister(2, size); 
					delete buf;
					IncreasePC();
					return;
				}
			
				if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
				{
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					
					machine->System2User(virtAddr, NewPos - OldPos, buf); 
					machine->WriteRegister(2, NewPos - OldPos);
				}
				else
				{
					machine->WriteRegister(2, -2);
				}
				delete buf;
				IncreasePC();
				return;
			}

			case SC_Write:
			{
				int virtAddr = machine->ReadRegister(4); 
				int charcount = machine->ReadRegister(5); 
				int id = machine->ReadRegister(6); 
				int OldPos;
				int NewPos;
				char *buf;
				if (id < 0 || id > 14)
				{
					printf("\nKhong the write vi id nam ngoai bang mo ta file.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				if (fileSystem->openf[id] == NULL)
				{
					printf("\nKhong the write vi file nay khong ton tai.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
				{
					printf("\nKhong the write file stdin hoac file only read.");
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				OldPos = fileSystem->openf[id]->GetCurrentPos(); 
				buf = machine->User2System(virtAddr, charcount);  
				if (fileSystem->openf[id]->type == 0)
				{
					if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
					{
						NewPos = fileSystem->openf[id]->GetCurrentPos();
						machine->WriteRegister(2, NewPos - OldPos);
						delete buf;
						IncreasePC();
						return;
					}
				}
				if (fileSystem->openf[id]->type == 3) 
				{
					int i = 0;
					while (buf[i] != 0 && buf[i] != '\n') 
					{
						gSynchConsole->Write(buf + i, 1); 
						i++;
					}
					buf[i] = '\n';
					gSynchConsole->Write(buf + i, 1); 
					machine->WriteRegister(2, i - 1); 
					delete buf;
					IncreasePC();
					return;
				}
			}
			case SC_CreateSemaphore:
			{
				int virtAddr = machine->ReadRegister(4);
				int semval = machine->ReadRegister(5);
				char* name = machine->User2System(virtAddr, MAX_BUFFER_SIZE);
				if(name == NULL)
				{
					DEBUG('a', "Not enough memory to create semaphore\n");
					printf("Not enough memory to create semaphore\n");
					machine->WriteRegister(2, -1);
					delete[]name;
					break;
				}
				int result = semTab->Create(name, semval);
				if(result == -1)
				{
					DEBUG('a', "Cannot initialize semaphore\n");
					printf("Cannot initialize semaphore\n");
					machine->WriteRegister(2, -1);
					delete[]name;
					return;
				}
				delete[]name;
				machine->WriteRegister(2, result);
				IncreasePC();
				break;
			}
			case SC_Up:
			{
				int virtAddr = machine->ReadRegister(4);
				char* name = machine->User2System(virtAddr, MAX_BUFFER_SIZE);
				if(name == NULL)
				{
					DEBUG('a', "Not enough memory\n");
					printf("Not enough memory\n");
					machine->WriteRegister(2, -1);
					delete[]name;
					break;
				}
				int result  = semTab->Signal(name);
				delete[]name;
				if (result==-1)
				{
					DEBUG('a', "Semaphore not found\n");
					printf("Semaphore not found\n");
					machine->WriteRegister(2, result);
					break;
				}
				machine->WriteRegister(2,result);
				IncreasePC();
				break;
			}
			case SC_Down:
			{
				int virtAddr = machine->ReadRegister(4);
				char* name = machine->User2System(virtAddr, MAX_BUFFER_SIZE);
				if(name == NULL)
				{
					DEBUG('a', "Not enough memory\n");
					printf("Not enough memory\n");
					machine->WriteRegister(2, -1);
					delete[]name;
					break;
				}
				int result  = semTab->Wait(name);
				delete[]name;
				if (result==-1)
				{
					DEBUG('a', "Semaphore not found\n");
					printf("Semaphore not found\n");
					machine->WriteRegister(2, result);
					break;
				}
				machine->WriteRegister(2,result);
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
