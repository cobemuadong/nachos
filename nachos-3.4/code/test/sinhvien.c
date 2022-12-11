#include "syscall.h"
#define false 0

int main()
{
    int sinhvienFD, voinuocFD, outputFD;
    int svCount;
    char charRead, prevChar;

    while (1)
    {
        Wait("mainProcess");
        prevChar = 1; // dummy value
        
        CreateFile("voinuoc.txt");
        sinhvienFD = Open("sinhvien.txt", 1);

        while(Read(&charRead, 1, sinhvienFD) > 0)
        {
            if (charRead < '0' || '9' < charRead)
            {
                // end of file
                if (charRead == '\0')
                {
                    Write("\0", 1, voinuocFD);
                    Write(" ", 1, outputFD);                    

                    Close(voinuocFD);
                    Close(outputFD);
                    
                    Signal("print_sinhvien");
                    Wait("print_voinuoc");
                    break;
                }
                else if ('0' <= prevChar && prevChar <= '9')
                {
                    prevChar = charRead;
                    Write(" ", 1, outputFD);

                    Close(voinuocFD);
                    Close(outputFD);

                    Signal("print_sinhvien");
                    Wait("print_voinuoc");
                    continue;
                }
                else
                {
                    prevChar = charRead;
                    continue;
                }
            }

            // reach a new number
            if (prevChar < '0' || prevChar < '9')
            {
                CreateFile("voinuoc.txt");
                voinuocFD = Open("voinuoc.txt", 0);
                outputFD = Open("output.txt", 0);

                //seek to end of output.txt
                while(Read(&prevChar, 1, outputFD) > 0)
                    continue;
            }

            // default case
            Write(&charRead, 1, voinuocFD);
            Write(&charRead, 1, outputFD);
            prevChar = charRead;

        }
        Close(sinhvienFD);

        // return to mainProcess
        Signal("subProcess");
    }

    Signal("subProcess");
    return 1;
}