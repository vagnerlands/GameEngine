#ifndef _MUTEXFACTORYWIN_H_
#define _MUTEXFACTORYWIN_H_

#include "CommonTypes.h"
#include "MutexFactory.h"
#include "CWinMutex.h"
class IMutex;
using namespace Types;

class MutexFactoryWin : public MutexFactory 
{
public:
    virtual ~MutexFactoryWin();

    MutexFactoryWin();

    static void Initialize();

    virtual IMutex* Create(const Byte* name) override;
};


#endif //_MUTEXFACTORYWIN_H_
