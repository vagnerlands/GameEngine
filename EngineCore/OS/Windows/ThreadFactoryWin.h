#ifndef _THREADFACTORYWIN_H_
#define _THREADFACTORYWIN_H_

#include "CommonTypes.h"
#include "ThreadFactory.h"
class IThread;
using namespace Types;

class ThreadFactoryWin : public ThreadFactory
{
public:
    virtual ~ThreadFactoryWin();

    ThreadFactoryWin();

    static void Initialize();

    virtual IThread* Create(const Byte* name, Byte affinity, void* EntryPoint) override;
};


#endif //_THREADFACTORYWIN_H_
