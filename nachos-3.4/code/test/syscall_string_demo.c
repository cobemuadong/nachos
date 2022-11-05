#include "syscall.h"

int main()
{
    char *buffer;

    PrintString("Nhap mot chuoi ky tu: ");
    ReadString(buffer,255);

    PrintString("Chuoi ky tu ban vua nhap la: ");
    PrintString(buffer);
    PrintChar('\n');
}