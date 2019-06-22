#ifndef _MUTEXFACTORY_H_
#define _MUTEXFACTORY_H_

#include "CommonTypes.h"

class IMutex;

using namespace Types;

// to be used on shared resources and avoid racing situations
class MutexFactory 
{
public:
    // singleton
    MutexFactory()
    {

    }
    virtual ~MutexFactory()
    {

    }
    virtual IMutex* Create(const Byte* mutexName) = 0;
    static MutexFactory& Instance()
    {
        // let it crash if the pointer isn't initialized
        return *s_pInstance;
    }

protected:
    // this singleton instance
    static MutexFactory* s_pInstance;

    MutexFactory& operator=(MutexFactory&);
};

#endif //_MUTEXFACTORY_H_
