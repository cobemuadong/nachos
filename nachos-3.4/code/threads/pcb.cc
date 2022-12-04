#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"


PCB::PCB(int id)
{
    if (id == 0)
    {
        this->parentID = -1;
    }
    else
    {
        this->parentID = currentThread->processID;
    }
    this->pid = id;

	this->numwait = this->exitcode = 0;
	this->thread = NULL;
    this->JoinStatus = -1;

	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->mutex = new Semaphore("mutex",1);
}
PCB::~PCB()
{
	
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(mutex != NULL)
		delete this->mutex;
	if(thread != NULL)
	{		
		thread->FreeSpace();
		thread->Finish();
	}
}

int PCB::GetID()
{ 
    return this->pid; 
}

int PCB::GetNumWait() 
{ 
    return this->numwait; 
}

int PCB::GetExitCode() 
{ 
    return this->exitcode; 
}

char* PCB::GetName()
{
    return thread->getName();
}

void PCB::SetExitCode(int ec)
{ 
    this->exitcode = ec; 
}

// Process tranlation to block
// Wait for JoinRelease to continue exec
void PCB::JoinWait()
{
	//Gọi joinsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ JoinRelease để thực hiện tiếp.
    joinsem->P();
}

// JoinRelease process calling JoinWait
void PCB::JoinRelease()
{ 
	// Gọi joinsem->V() để giải phóng tiến trình gọi JoinWait().
    joinsem->V();
}

// Let process tranlation to block state
// Waiting for ExitRelease to continue exec
void PCB::ExitWait()
{ 
	// Gọi exitsem-->V() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ ExitReleaseđể thực hiện tiếp.
    exitsem->P();
}

// Release wating process
void PCB::ExitRelease() 
{
	// Gọi exitsem-->V() để giải phóng tiến trình đang chờ.
    exitsem->V();
}

void PCB::IncNumWait()
{
	mutex->P();
	++numwait;
	mutex->V();
}

void PCB::DecNumWait()
{
	mutex->P();
	if(numwait > 0)
		--numwait;
	mutex->V();
}

int PCB::Exec(char* filename, int id)
{  
    // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	mutex->P();

    // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
	this->thread = new Thread(filename);

	if(this->thread == NULL){
		printf("\nPCB::Exec:: Not enough memory..!\n");
        mutex->V();
		return -1;
	}

	//  Đặt processID của thread này là id.
	this->pid = id;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	this->parentID = currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
 	this->thread->Fork(MyStartProcess,id);

    mutex->V();
	// Trả về id.
	return id;

}

void MyStartProcess(int pID)
{
	
	char *filename= pTab->GetName(pID);
	AddrSpace *space;
	space = new AddrSpace(filename);
	if(space == NULL)
	{
		printf("\nLoi: Khong du bo nho de cap phat cho tien trinh !!!\n");
		return; 
	}
	currentThread->space = space;

	space->InitRegisters();		// set the initial register values
	space->RestoreState();		// load page table register
	
	
	machine->Run();			// jump to the user progam

	ASSERT(FALSE);			// machine->Run never returns;
						// the address space exits
						// by doing the syscall "exit"
}