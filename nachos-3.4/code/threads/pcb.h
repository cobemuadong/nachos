#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block
class PCB
{
private:
    Semaphore *joinsem; // semaphore cho quá trình join
    Semaphore *exitsem; // semaphore cho quá trình exit    
    Semaphore *mutex;   // semaphore cho quá trình truy xuất độc quyền
    int exitcode;    
    Thread *thread;     // tiến trình của chương trình
    int pid;            //
    int numwait ;       // số tiến trình đã join
public:
    int parentID;   // ID cua tien trinh cha
    int JoinStatus; // Join Process ID, -1 if not

    PCB(int = 0);   // Contructor
    ~PCB();         // Destructor

    int Exec(char *filename, int pid); //nap chuong trinh co ten luu trong bien filename va processID se la pID
    int GetID();            // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();       // Trả về số lượng tiến trình chờ

    char* GetName();

    void JoinWait();    // 1. Tiến trình cha đợi tiến trình con kết thúc
    void ExitWait();    // 4. Tiến trình con kết thúc
    void JoinRelease(); // 2. Báo cho tiến trình cha thực thi tiếp
    void ExitRelease(); // 3. Cho phép tiến trình con kết thúc

    void IncNumWait(); // Tăng số tiến trình chờ
    void DecNumWait(); // Giảm số tiến trình chờ

    void SetExitCode(int); // Đặt exitcode của tiến trình
    int GetExitCode();     // Trả về exitcode
};

void MyStartProcess(int pID);

#endif // PCB_H