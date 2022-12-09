#include "syscall.h"

int main()
{   
    int inputFD, sinhvienFD;
    int sinhvienPID, voinuocPID;
    int timeCount = 0;
    char charRead;
    CreateSemaphore("process", 0);
    CreateSemaphore("print_sinhvien", 0);          
    CreateSemaphore("print_voinuoc", 1);
    CreateSemaphore("read_sinhvien.txt", 1);

    sinhvienPID = Exec("./test/sinhvien");
    voinuocPID = Exec("./test/voinuoc");
    Wait("process");
    // Join(voinuocPID);
    // Join(sinhvienPID);
    // Join(voinuocPID);



    // Create("output.txt");
    // inputFD = Open("input.txt", 0);

    // while(Read(&charRead, 1, inputFD) > 0)
    // {
    //     if(charRead == '\n')
    //         break;
    //     timeCount = timeCount * 10 + charRead - '0';
    // }

    // for(timeCount; timeCount > 0; timeCount--)
    // {
    //     // viet du lieu input vao sinhvien.txt
    //     Create("sinhvien.txt");
    //     sinhvienFD = Open("sinhvien.txt", 0);
    //     while(Read(&charRead, 1, inputFD) > 0)
    //     {
    //         if (charRead == '\n')
    //             break;
    //         Write(&charRead, 1, sinhvienFD);
    //     }
    //     Close(sinhvienFD);
    //     // PrintChar('\n');
    // }
    Halt();
    
}