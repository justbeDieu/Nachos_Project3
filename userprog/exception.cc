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

define MAX_INT32_LENGTH 512
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
void IncreasePC() //ham dung tang program counter
{
    int counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter + 4);
} 
void
ExceptionHandler(ExceptionType which)
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
				int numToRead = 0;   
				int numOfDigits = 0; 

				bool isInt = true; //kiem tra so nguyen
				bool isNegative = 0; //kiem tra so am hay duong

				char* buffer = new char[2]; 
				buffer[1] = 0;

				int StatusNum = 0; 

				while (numOfDigits < MAX_INT32_LENGTH) {
					StatusNum = gSynchConsole->Read(buffer, 1);
					if (StatusNum != 1) break;
					if ((buffer[0] < 48) || (buffer[0] > 57)) {
						if (numOfDigits == 0) {
							if (buffer[0] == '-') isNegative = 1;
							else {
								if (buffer[0] == ' ') continue;
								isInt = false;
								break;
							}
						}
						else {
							if (buffer[0] != ' ') isInt = false;
							else break;
						}
					}
					else {
						numToRead *= 10;
						numToRead += (buffer[0] - 48);
						++numOfDigits;
					}
				}
				numToRead *= isInt;
				numToRead *= (isNegative) == 0 ? 1 : -1;
				machine->WriteRegister(2, numToRead);
				delete[] buffer;
				IncreasePC(); //yeu cau nachos tang program counter
				break;
			}
			case SC_PrintInt: 
			{
				int number = machine->ReadRegister(4);
				if(number == 0) //neu so la 0 thi in ra man hinh so 0
				{
					gSynchConsole->Write("0", 1); 
							
				
				}
				else
				{
					bool isNegative = false; // mac dinh ban dau la so duong
					int numberOfNum = 0; // so chu so 
					int firstNumIndex = 0; 
					
					if(number < 0) //kiem tra neu la so am thi chuyen thanh so duong de tinh so chu so
					{
						isNegative = true;
						number *= -1;
						firstNumIndex = 1; 
					} 	
					
					//dem so chu so       
					int temp = number;
					while(temp)
					{
						numberOfNum++;
						temp /= 10;
					}
			
					// Tao buffer chuoi de in ra man hinh
					char* buffer;
					int MAX_BUFFER = 255;
					buffer = new char[MAX_BUFFER + 1];
					for(int i = firstNumIndex + numberOfNum - 1; i >= firstNumIndex; i--)
					{
						buffer[i] = (char)((number % 10) + 48);
						number /= 10;
					}
					if(isNegative)
					{
						buffer[0] = '-';
						buffer[numberOfNum + 1] = 0;
						gSynchConsole->Write(buffer, numberOfNum + 1);
						delete buffer;
					}
					else
					{
						buffer[numberOfNum] = 0;	
						gSynchConsole->Write(buffer, numberOfNum);
						delete buffer;
					}
					
				}
				IncreasePC(); //yeu cau nachos tang program counter
				break;      
			}
			case SC_ReadChar:
			{
				int maxBytes = 255;
				char* buffer = new char[255];
				int numBytes = gSynchConsole->Read(buffer, maxBytes);

				if(numBytes > 1) //Neu nhap nhieu hon 1 ky tu thi khong hop le
				{
					printf("Chi duoc nhap duy nhat 1 ky tu!");
					DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
					machine->WriteRegister(2, 0);
				}
				else if(numBytes == 0) //Ky tu rong
				{
					printf("Ky tu rong!");
					DEBUG('a', "\nERROR: Ky tu rong!");
					machine->WriteRegister(2, 0);
				}
				else
				{
					//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
					char c = buffer[0];
					machine->WriteRegister(2, c);
				}
				delete buffer;
				IncreasePC(); //yeu cau nachos tang program counter
				break;
			}
			case SC_PrintChar:
			{
				char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi r4
				gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
				IncreasePC(); //yeu cau nachos tang program counter
				break;
			}
			case SC_ReadString:
			{
				int addr = machine->ReadRegister(4); // Lay dia chi tham so buffer
				int length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi

				char* buffer = User2System(addr, length); //Copy chuoi tu User Space sang System Space
				gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
				System2User(addr, length, buffer); // Copy chuoi tu System Space sang User Space

				delete buffer; 
				IncreasePC(); //yeu cau nachos tang program counter
				break;
			}
			case SC_PrintString:
			{
				int addr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer
				char* buffer = User2System(addr, 255); // Copy chuoi tu User Space sang System Space voi kich thuoc 255
				int length = 0;
			
				while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
				gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi

				delete buffer;
				IncreasePC(); //yeu cau nachos tang program counter
				break;
			}
			default:
			{ 
				//printf("Unexpected user mode exception %d %d\n", which, type);
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
