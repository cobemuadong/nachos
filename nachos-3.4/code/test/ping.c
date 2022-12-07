#include "syscall.h"

int main ()
{
    int i = 10;
    while(i > 0)
    {
        PrintString("ping\n");
        i--;
    }
    return 0;
}