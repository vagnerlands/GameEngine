#include "MutexFactoryWin.h"
#include "CWinMutex.h"

MutexFactoryWin::~MutexFactoryWin()
{
}

MutexFactoryWin::MutexFactoryWin()
{
}

void MutexFactoryWin::Initialize()
{
    static MutexFactoryWin instance;
    MutexFactory::s_pInstance = &instance;
}

IMutex* MutexFactoryWin::Create(const Byte * name)
{
    CWinMutex* hnd = new CWinMutex();
    if (hnd != 0)
    {
        hnd->createMutex(name);
    }

    return hnd;
}

