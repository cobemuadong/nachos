// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MAX_FILE_LENGTH 32

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

bool isOverflow(char *buffer, int start, int end, int sign);

void IncreasePC()
{
    // registers[PrevPCReg] = registers[PCReg];
    int curr = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, curr);

    // registers[PCReg] = registers[NextPCReg];
    int next = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, next);

    // registers[NextPCReg] = pcAfter;
    int after = machine->ReadRegister(NextPCReg) + 4;
    machine->WriteRegister(NextPCReg, after);
}

/*
Input: - User space address (int)
 - Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/
char *User2System(int virtAddr, int limit)
{
    int i; // index
    int oneChar;
    char *kernelBuf = NULL;

    kernelBuf = new char[limit + 1]; // need for terminal string
    if (kernelBuf == NULL)
        return kernelBuf;
    memset(kernelBuf, 0, limit + 1);

    // printf("\n Filename u2s:");
    for (i = 0; i < limit; i++)
    {
        machine->ReadMem(virtAddr + i, 1, &oneChar);
        kernelBuf[i] = (char)oneChar;
        // printf("%c",kernelBuf[i]);
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

/*
Input: - User space address (int)
 - Limit of buffer (int)
 - Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User memory space
*/
int System2User(int virtAddr, int len, char *buffer)
{
    if (len < 0)
        return -1;
    if (len == 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do
    {
        oneChar = (int)buffer[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch (which)
    {
    case NoException:
    {
        return;
        break;
    }
    case PageFaultException:
    {
        DEBUG('a', "\n No valid translation found");
        printf("\n No valid translation found");
        interrupt->Halt();
        break;
    }
    case ReadOnlyException:
    {
        DEBUG('a', "\n Write attempted to page marked \"read-only\"");
        printf("\n Write attempted to page marked \"read-only\"");
        interrupt->Halt();
        break;
    }
    case BusErrorException:
    {
        DEBUG('a', "\n Translation resulted in an invalid physical address");
        printf("\n Translation resulted in an invalid physical address");
        interrupt->Halt();
        break;
    }
    case AddressErrorException:
    {
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
        printf("\n Unaligned reference or one that was beyond the end of the address space");
        interrupt->Halt();
        break;
    }
    case OverflowException:
    {
        DEBUG('a', "\n Integer overflow in add or sub.");
        printf("\n Integer overflow in add or sub.");
        interrupt->Halt();
        break;
    }
    case IllegalInstrException:
    {
        DEBUG('a', "\n Unimplemented or reserved instr.");
        printf("\n Unimplemented or reserved instr.");
        interrupt->Halt();
        break;
    }
    case NumExceptionTypes:
    {
        DEBUG('a', "\n Num Exception Types");
        printf("\n Num Exception Types");
        interrupt->Halt();
        break;
    }
    case SyscallException:
    {
        switch (type)
        {
        case SC_Halt:
        {
            DEBUG('a', "Shutdown, initiated by user program\n");
            interrupt->Halt();
            break;
        }
        case SC_ReadInt:
        {
            char *buffer = new char[256];
            int len = gSynchConsole->Read(buffer, 256);

            int result = 0, startIndex = 0, endIndex = 0;
            int sign = 1; // 1 - positive; -1 - negative

            if (buffer[0] == '-')
            {
                startIndex = 1;
                sign = -1;
            }

            for (int i = startIndex; i < len; i++)
            {
                if (buffer[i] != '0')
                    break;
                startIndex = i;
            }

            // transform char[] to int
            for (int i = startIndex; i < len; i++)
            {
                // .0000000000... case
                if (buffer[i] == '.')
                {
                    for (int j = i + 1; j < len; j++)
                        if (buffer[j] != '0')
                        {
                            machine->WriteRegister(2, 0);
                            delete[] buffer;
                            IncreasePC();
                            printf("Invalid input, not an integer!\n");
                            DEBUG('a', "Invalid input, not an integer!\n");
                            return;
                        }
                    endIndex = i - 1;
                    break;
                }

                // not even a number (!= 0,1,2,...,8,9)
                if (buffer[i] < '0' || buffer[i] > '9')
                {
                    machine->WriteRegister(2, 0);
                    delete[] buffer;
                    IncreasePC();
                    printf("Invalid input, not an integer!\n");
                    DEBUG('a', "Invalid input, not an integer!\n");
                    return;
                }

                result = result * 10 + (buffer[i] - 48) * sign;
                endIndex = i;
            }

            // overflow case
            if (isOverflow(buffer, startIndex, endIndex, sign) == true)
            {
                machine->WriteRegister(2, 0);
                delete[] buffer;
                IncreasePC();
                printf("Input exceeds the range of 'int' datatype [-2.147.483.648 - 2.147.483.647]\n");
                DEBUG('a', "Input exceeds the range of 'int' datatype [-2.147.483.648 - 2.147.483.647]\n");
                return;
            }

            machine->WriteRegister(2, result);
            delete[] buffer;
            break;
        }
        case SC_PrintInt:
        {
            int number = machine->ReadRegister(4);
            char *buffer = new char[11];

            int sign;
            if (number >= 0)
                sign = 1;
            else
                sign = -1;

            int index = 10;
            if (number == 0)
                gSynchConsole->Write("0", 1);

            while (number != 0)
            {
                buffer[index] = number % 10 * sign + 48;
                number /= 10;
                index--;
            }

            if (sign == -1)
            {
                buffer[index] = '-';
                index--;
            }
            index++;
            gSynchConsole->Write(buffer + index, 11 - index);

            delete[] buffer;
            break;
        }
        case SC_ReadString:
        {
            int virtAddr = machine->ReadRegister(4);
            int reserved_length = machine->ReadRegister(5);

            char *buffer = new char[reserved_length + 1];
            int actual_length = gSynchConsole->Read(buffer, reserved_length);

            buffer[actual_length] = '\0';

            System2User(virtAddr, actual_length + 1, buffer);
            delete[] buffer;
            break;
        }
        case SC_PrintString:
        {
            int virtAddr = machine->ReadRegister(4);
            char *buffer;

            buffer = User2System(virtAddr, 10000);

            int endIndex = 0;
            while (buffer[endIndex] != '\0' && endIndex <= 10000)
                endIndex++;

            gSynchConsole->Write(buffer, endIndex);
            delete[] buffer;
            break;
        }
        case SC_ReadChar:
        {
            char *buffer = new char[2];
            int bytes = gSynchConsole->Read(buffer, 2);
            if (bytes > 1)
            {
                printf("\nPlease just enter 1 letter");
                DEBUG('a', "\nPlease just enter 1 letter");
                machine->WriteRegister(2, 0);
            }
            else if (bytes == 0)
            {
                printf("\nYour input is empty!");
                DEBUG('a', "\nYour input is empty!");
                machine->WriteRegister(2, 0);
            }
            else if (bytes < 0)
            {
                printf("\nERROR: Console error\n");
                DEBUG('a', "\nERROR: Console error\n");
                machine->WriteRegister(2, 0);
            }
            else
            {
                machine->WriteRegister(2, buffer[0]);
            }
            delete[] buffer;
            break;
        }
        case SC_PrintChar:
        {
            char character = (char)machine->ReadRegister(4);
            gSynchConsole->Write(&character, 1);
            break;
        }
        case SC_Create:
        {
            int virtAddr;
            char *filename;

            virtAddr = machine->ReadRegister(4); // Read addr of filename from R4
            filename = User2System(virtAddr, MAX_FILE_LENGTH + 1);

            if (strlen(filename) == 0)
            {
                printf("\nFile name is not valid");
                DEBUG('a', "\nFile name is not valid");
                machine->WriteRegister(2, -1);
                delete[] filename;
                break;
            }

            if (filename == NULL)
            {
                printf("\n Not enough memory in system");
                DEBUG('a', "\n Not enough memory in system");
                machine->WriteRegister(2, -1);
                delete[] filename;
                break;
            }

            if (!fileSystem->Create(filename, 0))
            {
                printf("\nError create file '%s'", filename);
                DEBUG('a', "\nError create file '%s'", filename);
                machine->WriteRegister(2, -1);
                delete[] filename;
                break;
            }

            printf("\nCreate file '%s' success\n", filename);
            DEBUG('a', "\nCreate file '%s' success\n", filename);
            machine->WriteRegister(2, 0);
            delete[] filename;
            break;
        }
        case SC_Open:
        {
            int virtAddr = machine->ReadRegister(4);
            char *filename = User2System(virtAddr, MAX_FILE_LENGTH);
            int type = machine->ReadRegister(5);
            int freeSlot = fileSystem->FindFreeSlot();

            if (freeSlot < 0)
            {
                printf("\nError");
                DEBUG('d', "\nError");
                delete[] filename;
                break;
            }
            if (type == 0 || type == 1)
            {
                fileSystem->GetOpenTable()[freeSlot] = fileSystem->Open(filename, type);
                if ((fileSystem->GetOpenTable()[freeSlot] != NULL))
                {
                    machine->WriteRegister(2, freeSlot);
                }
                else
                {
                    printf("\n ERROR");
                    DEBUG('a', "\n ERROR");
                    machine->WriteRegister(2, -1);
                }
            }
            else if (type == 2)
            {
                machine->WriteRegister(2, 0);
            }
            else if (type == 3)
            {
                machine->WriteRegister(2, 1);
            }
            else
            {
                machine->WriteRegister(2, -1);
            }
            delete[] filename;
            break;
        }
        case SC_Close:
        {
            int openFileId = machine->ReadRegister(4);
            if (openFileId >= 0 && openFileId <= 9)
            {
                if (fileSystem->GetOpenTable()[openFileId])
                {
                    delete fileSystem->GetOpenTable()[openFileId];
                    fileSystem->GetOpenTable()[openFileId] = NULL;
                    machine->WriteRegister(2, 0);
                    break;
                }
            }
            machine->WriteRegister(2, -1);
            break;
        }
        case SC_Read:
        {
            int virtAddr = machine->ReadRegister(4);
            int charCount = machine->ReadRegister(5);
            int id = machine->ReadRegister(6);
            int oldPos;
            int newPos;
            char *buf;
            int fileLength;

            if (id < 0 || id > 9)
            {
                printf("\nCan't read!! File id out of range");
                DEBUG('a', "\nCan't read!! File id out of range");
                machine->WriteRegister(2, -1);
                break;
            }

            if (fileSystem->GetOpenTable()[id] == NULL)
            {
                printf("\nThis file is not exist!!");
                DEBUG('a', "\nThis file is not exist");
                machine->WriteRegister(2, -1);
                break;
            }

            buf = User2System(virtAddr, charCount);
            if (fileSystem->GetOpenTable()[id]->GetType() == 3)
            {
                printf("\nCan't Read!! File's type is stdout");
                DEBUG('a', "\nCan't Read!! File's type is stdout");
                machine->WriteRegister(2, -1);
                delete[] buf;
                break;
            }

            if (fileSystem->GetOpenTable()[id]->GetType() == 2)
            {   
                int size = gSynchConsole->Read(buf, charCount); 
				System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
                machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
                delete[] buf;
                break;
            }

            oldPos = fileSystem->GetOpenTable()[id]->GetCurrentPos();
            if (fileSystem->GetOpenTable()[id]->Read(buf, charCount) > 0)
            {
                newPos = fileSystem->GetOpenTable()[id]->GetCurrentPos();
                fileLength = newPos - oldPos;
                System2User(virtAddr, fileLength, buf);
                machine->WriteRegister(2, fileLength);
            }
            else
            {
                DEBUG('a', "\nEmpty File!");
                machine->WriteRegister(2, -2);
            }
            delete[] buf;
            break;
        }
        case SC_Write:
        {
            int virtAddr = machine->ReadRegister(4);
            int charCount = machine->ReadRegister(5);
            int id = machine->ReadRegister(6);
            int oldPos;
            int newPos;
            int fileLength;
            char *buf;

            if (id < 0 || id > 9)
            {
                printf("\nCan't write!! File id out of range");
                DEBUG('a', "\nCan't write!! File id out of range");
                machine->WriteRegister(2, -1);
                break;
            }

            if (fileSystem->GetOpenTable()[id] == NULL)
            {
                printf("\nCan't write!! File is not open or not exist!");
                DEBUG('a', "\nCan't write!! File is not open or not exist!");
                machine->WriteRegister(2, -1);
                break;
            }

            if (fileSystem->GetOpenTable()[id]->GetType() == 1 || fileSystem->GetOpenTable()[id]->GetType() == 2)
            {
                printf("\nCan't write!! stdin or Read Only mode!");
                DEBUG('a', "\nCan't write!! stdin or Read Only mode!");
                machine->WriteRegister(2, -1);
                break;
            }

            oldPos = fileSystem->GetOpenTable()[id]->GetCurrentPos();
            buf = User2System(virtAddr, charCount);
            if (fileSystem->GetOpenTable()[id]->GetType() == 0)
            {
                if (fileSystem->GetOpenTable()[id]->Write(buf, charCount) > 0)
                {
                    newPos = fileSystem->GetOpenTable()[id]->GetCurrentPos();
                    fileLength = newPos - oldPos;
                    machine->WriteRegister(2, fileLength);
                    delete[] buf;
                    break;
                }
            }

            if (fileSystem->GetOpenTable()[id]->GetType() == 3)
            {
                int i = 0;
                while (buf[i] != 0 && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
                {
                    gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole
                    i++;
                }
                buf[i] = '\n';
                gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
                machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
                delete[]buf;
                break;;
            }
        }
        case SC_Exec:
        {  
            int virtAddr = machine->ReadRegister(4);
            char* name = User2System(virtAddr, MAX_FILE_LENGTH + 1);

            if(name == NULL)
            {
                DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
                break;
            }

            OpenFile* openFile = fileSystem->Open(name);
            
            if(openFile == NULL){
                printf("\nExec:: Can't open this file.");
                DEBUG('a',"\nExec:: Can't open this file.");
				machine->WriteRegister(2,-1);
				break;
            }

            delete openFile;

            int pid = pTab->ExecUpdate(name);
            machine->WriteRegister(2, pid);
            delete[]name;
            break;
        }
        case SC_Join:
        {
            int pid = machine->ReadRegister(4);
            int result = pTab->JoinUpdate(pid);
            machine->WriteRegister(2,result);
            break;
        }
        case SC_Exit:
        {
            int exitStatus = machine->ReadRegister(4);
            int result = pTab->ExitUpdate(exitStatus);
            machine->WriteRegister(2,result);
            currentThread->FreeSpace();
            currentThread->Finish();
            break;
        }
        case SC_CreateSemaphore:
        {
            int virtAddr = machine->ReadRegister(4);
            int semval = machine->ReadRegister(5);
            char* name = User2System(virtAddr, MAX_FILE_LENGTH + 1);

            if(name == NULL){
                DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
                break;
            }

            int result = semTab->Create(name, semval);
            if(result < 0){
                printf("\nExec:: Error");
                DEBUG('a',"\nExec:: Error");
                machine->WriteRegister(2, -1);
                delete[]name;
                break;
            }
            delete[]name;
            machine->WriteRegister(2,result);
            break;
        }
        case SC_Wait:
        {
            int virtAddr = machine->ReadRegister(4);
            char* name = User2System(virtAddr, MAX_FILE_LENGTH + 1);

            if(name == NULL){
                DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
                break;
            }

            int result = semTab->Wait(name);

            if(result == -1){
                DEBUG('a', "\nSemaphore not found!");
				printf("\n Semaphore not found!");
				machine->WriteRegister(2, -1);
				delete[] name;
				break;	
            }

            machine->WriteRegister(2, result);
            delete[]name;
            break;
        }
        case SC_Signal:
        {
            int virtAddr = machine->ReadRegister(4);
            char* name = User2System(virtAddr, MAX_FILE_LENGTH + 1);

            if(name == NULL){
                DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
                break;
            }

            int result = semTab->Signal(name);

            if(result == -1){
                DEBUG('a', "\nSemaphore not found!");
				printf("\nSemaphore not found!");
				machine->WriteRegister(2, -1);
				delete[] name;
				break;	
            }

            machine->WriteRegister(2, result);
            delete[]name;
            break;
        }
        }
        IncreasePC();
        break;
    }
    }
}

bool isOverflow(char *buffer, int start, int end, int sign)
{
    if ((end - start + 1) > 10)
        return true;

    if ((end - start + 1) < 10)
        return false;

    char max[] = {"214748364"}; // without the unit digit
    for (int i = 0; i < 9; i++)
    {
        if (buffer[start + i] > max[i])
            return true;

        if (buffer[start + i] < max[i])
            return false;
    }

    if (sign == 1)
    {
        if (buffer[start + 9] <= '7')
            return false;
    }
    else
    {
        if (buffer[start + 9] <= '8')
            return false;
    }

    return true;
}
