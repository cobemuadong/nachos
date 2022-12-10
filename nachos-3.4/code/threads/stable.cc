#include "stable.h"

// Constructor
STable::STable()
{
    this->bm = new BitMap(MAX_SEMAPHORE);

    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        this->lockTab[i] = NULL;
    }
}

// Destructor
STable::~STable()
{
    if (this->bm)
    {
        delete this->bm;
        this->bm = NULL;
    }
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (this->lockTab[i])
        {
            delete this->lockTab[i];
            this->lockTab[i] = NULL;
        }
    }
}

int STable::Create(char *name, int init)
{

    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
        {
            if (strcmp(name, lockTab[i]->GetName()) == 0)
            {
                printf("\nSemaphore already exist!");
                return -1;
            }
        }
    }

    int id = this->FindFreeSlot();

    if (id < 0)
    {
        printf("\nNo Space left!");
        return -1;
    }

    printf("\nCreate semaphore success %d",id);
    this->lockTab[id] = new Sem(name, init);
    return 0;
}

int STable::Wait(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
        {
            if (strcmp(name, lockTab[i]->GetName()) == 0)
            {
                lockTab[i]->wait();
                return 0;
            }
        }
    }
    printf("\nSemaphore not found");
    DEBUG('a',"\nSemaphore not found");
    return -1;
}

int STable::Signal(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
        {
            if (strcmp(name, lockTab[i]->GetName()) == 0)
            {
                lockTab[i]->signal();
                return 0;
            }
        }
    }
    printf("\nSemaphore not found");
    DEBUG('a',"\nSemaphore not found");
    return -1;
}

int STable::FindFreeSlot()
{
    return this->bm->Find();
}