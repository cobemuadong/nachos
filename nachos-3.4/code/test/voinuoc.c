#include "syscall.h"

int main()
{
    int voinuocFD, outputFD;
    int dungtichBinh = 0;
    int voi1 = 0, voi2 = 0;
    char charRead;

    // each time in mainProcess loop
    while (1)
    {
        voi1 = 0;
        voi2 = 0;

        // each student in ont time loop
        while (1)
        {
            Wait("print_sinhvien");
            dungtichBinh = 0;
            voinuocFD = Open("voinuoc.txt", 1);
            outputFD = Open("output.txt", 0);

            // seek to the end of output.txt
            while (Read(&charRead, 1, outputFD) > 0)
                continue;

            while (Read(&charRead, 1, voinuocFD) > 0)
            {
                if(charRead == '\0')
                    break;
                dungtichBinh = dungtichBinh * 10 + charRead - '0';
            }

            if (voi1 == 0)
            {
                voi1 = dungtichBinh;
                Write("1", 1, outputFD);
            }
            else
            {
                voi2 = dungtichBinh;
                Write("2", 1, outputFD);                
            }

            // rot' nuoc' simulator
            if (voi1 >= voi2)
            {
                voi1 = voi1 - voi2;
                voi2 = 0;
            }
            else
            {
                voi2 = voi2 - voi1;
                voi1 = 0;
            }

            // het thoi diem
            if (charRead == '\0')
            {
                Write("\n", 1, outputFD);
                Close(voinuocFD);
                Close(outputFD);
                Signal("print_voinuoc");
                break;
            }
            else
            {
                Write("    ", 4, outputFD);
                Close(voinuocFD);
                Close(outputFD);
                Signal("print_voinuoc");
            }
            
        }
    }
}