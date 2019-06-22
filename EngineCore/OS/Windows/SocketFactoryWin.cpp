#include "SocketFactoryWin.h"
#include <Windows.h>
#undef UNICODE
#define WIN32_LEAN_AND_MEAN


SocketFactory* SocketFactory::s_pInstance = nullptr;

#include "CWinSocket.h"

SocketFactoryWin::~SocketFactoryWin()
{
    // close all socket operations for this application
    WSACleanup();
}

SocketFactoryWin::SocketFactoryWin()
{
}

void SocketFactoryWin::Initialize()
{
    // for windows this is required to do when working with sockets
    WSADATA wsData;
    // uses socket version 2.2 (Big endian)
    UInt16 version = MAKEWORD(2, 2);
    // starts up
    (void)WSAStartup(version, &wsData);

    cout << "[Socket]" << endl;
    cout << "  [info] Description: " << wsData.szDescription << endl;
    cout << "  [info] System Status: " << wsData.szSystemStatus << endl;
    cout << "  [info] Version: " << wsData.wVersion << endl;

    static SocketFactoryWin instance;
    SocketFactory::s_pInstance = &instance;
}

ISocket* SocketFactoryWin::Create(const Byte * mutexName)
{
    CWinSocket* hnd = new CWinSocket();
    if (hnd != 0)
    {
        //hnd->createMutex(mutexName);
    }

    return hnd;
}

