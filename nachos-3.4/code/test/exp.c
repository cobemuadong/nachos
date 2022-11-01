/*
 * Experiment space
 */
#include <syscall.h>

int main()
{
    char *buffer;
    PrintString("Enter your name: ");
    ReadString(buffer, 255);
    PrintString("Name: ");
    PrintString(buffer);

    Halt();
}