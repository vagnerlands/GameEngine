#ifndef _SOCKETUTILS_H_
#define _SOCKETUTILS_H_

#include "CommonTypes.h"

using namespace Types;

class SocketUtils
{
public:
    virtual ~SocketUtils();

    static bool IsMulticastIp(const Byte* pAddress);

private:
    // disable unwanted instantiation of this class
    SocketUtils();
    // disable unwanted copy 
    SocketUtils& operator=(SocketUtils&);

};

#endif //_SOCKETUTILS_H_
