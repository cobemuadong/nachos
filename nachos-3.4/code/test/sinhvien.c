#include "syscall.h"
#define false 0

int main()
{
    int sinhvienFD, voinuocFD, outputFD;
    int svCount;
    int pA[100];
    int isNotBlank;
    char charRead, prevChar;

    while (1)
    {
        Wait("mainProcess");
        isNotBlank = false; // file is empty by default
        prevChar = 1; // dummy value
        
        Create("voinuoc.txt");
        sinhvienFD = Open("sinhvien.txt", 1);

        while(Read(&charRead, 1, sinhvienFD) > 0)
        {
            // check if file is not empty
            isNotBlank++;
            PrintChar(charRead);
            // reach a character that is not a number
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
                Create("voinuoc.txt");
                voinuocFD = Open("voinuoc.txt", 0);
                outputFD = Open("output.txt", 0);

                //seek to end of output.txt
                while(Read(&prevChar, 1, outputFD) > 0)
                    continue;
            }

            // normal works
            Write(&charRead, 1, voinuocFD);
            Write(&charRead, 1, outputFD);
            prevChar = charRead;

        }
        Close(sinhvienFD);


        // end of mainProcess
        if (isNotBlank != false)
            break;
        PrintString("hihi");
        Signal("subProcess");
    }

    Signal("subProcess");
    return 1;
}