#ifndef _SOCKETFACTORY_H_
#define _SOCKETFACTORY_H_

#include "CommonTypes.h"

class ISocket;

using namespace Types;

// to be used on shared resources and avoid racing situations
class SocketFactory
{
public:
    // singleton
    SocketFactory()
    {

    }
    virtual ~SocketFactory()
    {

    }
    virtual ISocket* Create(const Byte* name) = 0;
    static SocketFactory& Instance()
    {
        // let it crash if the pointer isn't initialized
        return *s_pInstance;
    }

protected:
    // this singleton instance
    static SocketFactory* s_pInstance;

    SocketFactory& operator=(SocketFactory&);
};


#endif //_SOCKETFACTORY_H_
