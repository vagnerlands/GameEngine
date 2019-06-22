#ifndef _THREADFACTORY_H_
#define _THREADFACTORY_H_

#include "CommonTypes.h"

class IThread;

using namespace Types;

// to be used on shared resources and avoid racing situations
class ThreadFactory
{
public:
    // singleton
    ThreadFactory()
    {

    }
    virtual ~ThreadFactory()
    {

    }
    virtual IThread* Create(const Byte* name, void* EntryPoint) = 0;
    static ThreadFactory& Instance()
    {
        // let it crash if the pointer isn't initialized
        return *s_pInstance;
    }

protected:
    // this singleton instance
    static ThreadFactory* s_pInstance;

    ThreadFactory& operator=(ThreadFactory&);
};


#endif //_THREADFACTORY_H_
