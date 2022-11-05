#include "syscall.h"

int main()
{
	int i;
	PrintString("========== Bang ma Ascii ==========\n");
	PrintString("He thap phan\tKy tu\n");

	for (i = 32; i < 127; i++)
	{
		PrintChar('\t');
		PrintInt(i);
		PrintChar('\t');
		PrintChar(i);
		PrintChar('\n');
	}

	return 0;
}
