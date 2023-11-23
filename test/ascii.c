#include "syscall.h"


int main()
{
	int i;
	char c;
	for (i = 32; i < 127; i++)
	{
		PrintInt(i);
		PrintChar(':');
		PrintChar(' ');
		c = (char)(i);
		PrintChar(c);
		PrintChar('\t');
		if (i % 8 == 0)
			PrintChar('\n');
	}
	Halt();
}
