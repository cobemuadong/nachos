#include "syscall.h"

int main()
{
    int sinhvienFD;
    int pA[100];
    int sinhvienCount = 1, i = 0;
    char charRead;
    int voi1 = 0, voi2 = 0;

    //store sinhvien data
    Wait("read_sinhvien.txt");
    sinhvienFD = Open("sinhvien.txt", 1);
    i = 0;
    pA[i] = 0;
    while(Read(&charRead, 1, sinhvienFD) > 0)
    {
        if(charRead < '0' || charRead > '9')
        {
            // skip character that is not a number
            while(Read(&charRead, 1, sinhvienFD) > 0)
                if (charRead >= '0' && charRead <= '9')
                    break;
            
            // end of file detected
            if(charRead < '0' || charRead > '9')
                break;

            // new number encountered
            sinhvienCount++;     
            i++;
            pA[i] = 0;
        }
        pA[i] = pA[i] * 10 + charRead - '0';
    }
    Close(sinhvienFD);
    Signal("read_sinhvien.txt");


    // xu ly voi nuoc
    for(i = 0; i < sinhvienCount; i++)
    {
        Wait("print_sinhvien");
        if (voi1 == 0)
        {
            voi1 = pA[i];
            PrintString("1\n");            
        }
        else
        {
            voi2 = pA[i];
            PrintString("2\n");            
        }
        
        while(voi1 > 0 && voi2 > 0)
        {
            voi1--;
            voi2--;
        }
        Signal("print_voinuoc");
    }

    Signal("process");
}