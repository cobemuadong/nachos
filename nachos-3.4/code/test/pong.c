#include "syscall.h"

int main ()
{
    int i =10;
    while(i)
    {
        PrintString("pong\n");
        i = i-1;
    }
    return 0;
}