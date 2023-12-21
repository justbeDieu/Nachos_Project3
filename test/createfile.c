#include "syscall.h"
#define MAX_LENGTH 512

int main()
{
	int stdin;
	char fileName[MAX_LENGTH];
	PrintString("\nTao File:\n");
	PrintString("\nNhap ten file tu stdin: ");
	stdin = Open("stdin", 2); //Open File stdin for writting name File
	if (stdin != -1)
	{
        //Read name File 
		int len = Read(fileName, MAX_LENGTH, stdin); 
		if(len <= 1)
			PrintString("\n!!Ten file khong hop le!\n");
		else
		{
			if (CreateFile(fileName) == 0) 
			{
				PrintString("\nTao file thanh cong.\n");
			}
            else
            {
                PrintString("\nLoi khi tao file.\n");
            }
		}
		Close(stdin); //Close File stdin
	}
	return 0;
}
