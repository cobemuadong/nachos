#include "syscall.h"

int main ()
{
    int i = 1000;
    while(i > 0)
    {
        Wait("pong");
        PrintString("ping\n");
        i--;
        Signal("ping");
    }
    return 0;
}