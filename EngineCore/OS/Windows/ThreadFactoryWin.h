#ifndef _SOCKETFACTORYWIN_H_
#define _SOCKETFACTORYWIN_H_

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

    virtual IThread* Create(const Byte* name) override;
};


#endif //_SOCKETFACTORYWIN_H_
