#include "syscall.h"

int main()
{
    int number;

    PrintString("Hay nhap mot so nguyen: ");
    number = ReadInt();
    
    
    PrintString("So ban vua nhap la: ");
    PrintInt(number);
    PrintChar('\n');
}