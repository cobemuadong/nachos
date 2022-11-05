#include "syscall.h"

int main()
{
	int i;
	char c;
	PrintString("Bang ma ascii\n");
	
	for (i = 32; i < 127; i++)
	{
		c = (char)i;
		PrintInt(i);
		PrintChar('.');
		PrintChar(c);
		PrintChar('\t');
		if ((i - 31) % 10 == 0)
			PrintChar('\n');
	}
	PrintChar('\n');
	return 0;
}