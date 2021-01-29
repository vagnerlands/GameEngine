#ifndef _SemaphoreFactory_H_
#define _SemaphoreFactory_H_

#include "CommonTypes.h"

class ISemaphore;

using namespace Types;

// to be used on shared resources and avoid racing situations
class SemaphoreFactory
{
public:
    // singleton
    SemaphoreFactory()
    {

    }
    virtual ~SemaphoreFactory()
    {

    }
    virtual ISemaphore* Create(const Byte* name, Int32 count) = 0;
    static SemaphoreFactory& Instance()
    {
        // let it crash if the pointer isn't initialized
        return *s_pInstance;
    }

protected:
    // this singleton instance
    static SemaphoreFactory* s_pInstance;

    SemaphoreFactory& operator=(SemaphoreFactory&) = delete;
};


#endif //_SemaphoreFactory_H_
