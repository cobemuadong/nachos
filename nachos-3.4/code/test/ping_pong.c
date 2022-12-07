#include "syscall.h"

int main()
{
    int pingID = Exec("./test/ping");
    int pongID = Exec("./test/pong");
    CreateSemaphore("ping", 0);
    CreateSemaphore("pong", 1);
    Join(pingID);
    Join(pongID);
    Halt();
}