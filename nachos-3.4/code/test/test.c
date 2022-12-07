#include "syscall.h"

int main()
{
    // CreateSemaphore("ping", 0);
    // CreateSemaphore("pong", 0);

    int pingID = Exec("./test/ping");
    // int pongID = Exec("./test/pong");
    int pingExitCode = Join(pingID);
    // // Join(pongID);
    // Halt();
    
    // int inputFileDescriptor, outputFileDescriptor;
    // int numberOfTimesHandle;
    // int isSuccess;
    // char charRead;

    // Create("output.txt");

	// // Mo file input.txt chi de doc
	// inputFileDescriptor = Open("input.txt", 1);
	// if(inputFileDescriptor == -1)
	// 	return 1;
	
	// // Mo file output.txt de doc va ghi
	// outputFileDescriptor = Open("output.txt", 0);
	// if(outputFileDescriptor == -1)
	// {
	// 	Close(inputFileDescriptor);
	// 	return 1;
	// }

    // // Doc so thoi diem can xet
    // numberOfTimesHandle = 0;
    // while(1)
    // {
    //     Read(&charRead, 1, inputFileDescriptor);
    //     if(charRead == '\n')
    //         break;
    //     numberOfTimesHandle = numberOfTimesHandle * 10 + charRead - '0';
    // }
    // char*buffer;
    // int times;
    // int filltime_per_liter = 10;
    // int file_input = Open("input.txt", 0);
    // if (file_input == -1) {
    //     PrintString("Cannot open file input.txt\n");
    //     return 1;
    // }
    
    // Read(buffer, 1, file_input);
    
    // times = (buffer[0] - '0');

    // PrintInt(times);
    // PrintChar('\n');

    
}