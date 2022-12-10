#include "syscall.h"

int main()
{   
    int inputFD, sinhvienFD;
    int sinhvienPID, voinuocPID;
    int timeCount;
    char charRead;

    // tao cac semaphore
    CreateSemaphore("mainProcess", 0);
    CreateSemaphore("subProcess", 0);
    CreateSemaphore("print_sinhvien", 0);          
    CreateSemaphore("print_voinuoc", 0);
    CreateSemaphore("open_sinhvien.txt", 1);

    Create("output.txt");
    Create("sinhvien.txt");
    inputFD = Open("input.txt", 1);
    if (inputFD == -1)
    {
        PrintString("\nCo loi khi mo file input.txt...");
        return 0;
    }

    // doc so thoi diem uong nuoc
    timeCount = 0;
    while(Read(&charRead, 1, inputFD) > 0)
    {
        if(charRead == '\n')
            break;
        timeCount = timeCount * 10 + charRead - '0';
    }

    sinhvienPID = Exec("./test/sinhvien");
    if (sinhvienPID == -1)
    {
        PrintString("\nCo loi khi mo chuong trinh sinh vien...");
        Close(inputFD);
        return 0;
    }

    voinuocPID = Exec("./test/voinuoc");
    if (sinhvienPID == -1)
    {
        PrintString("\nCo loi khi mo chuong trinh voi nuoc...");
        Close(inputFD);
        return 0;
    }


    while (timeCount > 0)
    {
        // luu thong tin binh nuoc cua sinh vien tai 1 thoi diem vao file sinhvien.txt
        Wait("open_sinhvien.txt");
        sinhvienFD = Open("sinhvien.txt", 0);
        if (sinhvienFD == -1)
        {
            PrintString("\nCo loi khi mo file sinhvien.txt...");
            Close(inputFD);
            return 0;
        }
        while(Read(&charRead, 1, inputFD) > 0)
        {
            if(charRead == '\n')
                break;
            Write(&charRead, 1, sinhvienFD);
        }
        Write("\0", 1, sinhvienFD);
        Close(sinhvienFD);
        Signal("open_sinhvien.txt");
        // cho phep chuong trinh sinh vien, voi nuoc chay.
        Signal("mainProcess");

        // doi chuong trinh sinh vien, voi nuoc chay xong
        Wait("subProcess");

        Create("sinhvien.txt");
        timeCount--;
    }
    Close(inputFD);
    Exit(sinhvienPID);
    Exit(voinuocPID);
}