#include "syscall.h"

int main()
{
    char*buffer;
    int times;
    int filltime_per_liter = 10;
    int file_input = Open("input.txt", 0);
    if (file_input == -1) {
        PrintString("Cannot open file input.txt\n");
        return 1;
    }
    
    Read(buffer, 1, file_input);
    
    times = (buffer[0] - '0');

    PrintInt(times);
    PrintChar('\n');

    
}