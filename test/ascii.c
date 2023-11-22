#include "syscall.h"


int main()
{
	int i;
	char c;
	for (i = 32; i < 127; i++)
	{
		PrintInt(i);
		printChar(":");
		printChar(" ");
		c = (char)(i);
		PrintChar(c);
		printChar("\t");
		if (i % 8 == 0)
			printChar("\n");
	}
	Halt();
}