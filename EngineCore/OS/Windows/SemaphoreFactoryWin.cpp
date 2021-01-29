#include "SemaphoreFactoryWin.h"
#include "CWinSemaphore.h"

SemaphoreFactory* SemaphoreFactory::s_pInstance = nullptr;

SemaphoreFactoryWin::~SemaphoreFactoryWin()
{
}

SemaphoreFactoryWin::SemaphoreFactoryWin()
{
}

void SemaphoreFactoryWin::Initialize()
{
    static SemaphoreFactoryWin instance;
    SemaphoreFactory::s_pInstance = &instance;
}

ISemaphore* SemaphoreFactoryWin::Create(const Byte * name, Int32 count)
{
    CWinSemaphore* hnd = new CWinSemaphore();
    if (hnd != 0)
    {
        hnd->createSemaphore(name, count);
    }

    return hnd;
}

