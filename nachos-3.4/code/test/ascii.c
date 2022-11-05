#include "syscall.h"

int main()
{
	int i;
	PrintString("Bang ma ascii\n");

	for (int i = 32; i < 127; i++)
	{
		c = char(i);
		PrintInt(i);
		PrintChar('\t');
		PrintChar(c);
		PrintChar('\n');
	}

	return 0;
}
