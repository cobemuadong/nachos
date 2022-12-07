#include "syscall.h"

int main ()
{
    int i =1000;
    while(i)
    {
        Wait("ping");
        PrintString("pong\n");
        i--;
        Signal("pong");
    }
    return 0;
}