#ifndef _SemaphoreFactoryWin_H_
#define _SemaphoreFactoryWin_H_

#include "CommonTypes.h"
#include "SemaphoreFactory.h"
class IThread;
using namespace Types;

class SemaphoreFactoryWin : public SemaphoreFactory
{
public:
    virtual ~SemaphoreFactoryWin();

    SemaphoreFactoryWin();

    static void Initialize();

    virtual class ISemaphore* Create(const Byte* name, Int32 count) override;
};


#endif //_SemaphoreFactoryWin_H_
