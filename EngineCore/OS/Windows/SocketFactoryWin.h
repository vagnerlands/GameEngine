#ifndef _SOCKETFACTORYWIN_H_
#define _SOCKETFACTORYWIN_H_

#include "CommonTypes.h"
#include "SocketFactory.h"
#include "CWinSocket.h"
class ISocket;
using namespace Types;

class SocketFactoryWin : public SocketFactory
{
public:
    virtual ~SocketFactoryWin();

    SocketFactoryWin();

    static void Initialize();

    virtual ISocket* Create(const Byte* name) override;
};


#endif //_SOCKETFACTORYWIN_H_
