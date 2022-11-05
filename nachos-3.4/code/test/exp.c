/*
 * Experiment space
 */
#include <syscall.h>

int main()
{
    char buffer;
    buffer = ReadChar();
    PrintChar(buffer);

    Halt();
}