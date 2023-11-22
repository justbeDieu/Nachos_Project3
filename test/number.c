/* number.c 
 *    Test program to input and ouput int.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"


int main()
{
	PrintInt(ReadInt());
	Halt();
}
