#include "ThreadFactoryWin.h"
#include "CWinThread.h"

ThreadFactory* ThreadFactory::s_pInstance = nullptr;

ThreadFactoryWin::~ThreadFactoryWin()
{
}

ThreadFactoryWin::ThreadFactoryWin()
{
}

void ThreadFactoryWin::Initialize()
{
    static ThreadFactoryWin instance;
    ThreadFactory::s_pInstance = &instance;
}

IThread* ThreadFactoryWin::Create(const Byte * name, void* EntryPoint)
{
    CWinThread* hnd = new CWinThread();
    if (hnd != 0)
    {
        hnd->createThread(name, EntryPoint);
    }

    return hnd;
}

