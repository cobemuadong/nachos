#include "syscall.h"

int main()
{
    int pingPID, pongPID;
	PrintString("Ping-Pong test starting...\n\n");
	CreateSemaphore("ping", 0);
	CreateSemaphore("pong", 1);
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong"); 
	Join(pingPID);
	Join(pongPID); 
}