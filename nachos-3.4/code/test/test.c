#include "syscall.h"

int main()
{
    int output;
    int a = 1;
    Create("sinhvien.txt");
    output = Open("sinhvien.txt",0);
    Write((char*)&a,1,output);
    


    
}   