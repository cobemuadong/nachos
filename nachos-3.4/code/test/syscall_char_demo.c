#include "syscall.h"

int main()
{
    char character;

    PrintString("Nhap mot ky tu: ");
    character = ReadChar();

    PrintString("Ky tu ban vua nhap: ");
    PrintChar(character);
    PrintChar('\n');
}