#ifndef _CWINSOCKET_H_
#define _CWINSOCKET_H_

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include "CommonTypes.h"
#include "ISocket.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace Types;

class CWinSocket : public ISocket 
{
public:

    // Socket constructor
	CWinSocket();
    // Socket destructor
	~CWinSocket();
	// create a socket based on given attributes
    ESocketCreationStatus Create(const SSocketAttributes& socketAttributes);

    Int32 Bind();
    Int32 Bind(UInt16 port, const Byte* pIp);

    Int32 Write(const Byte* pIp, UInt32 port, const Byte* buffer, Int32 const size);
	
	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// content of buffer till it's size will be copied
	Int32 Write(const Byte* buffer, Int32 const size);

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// the buffer will be copied to Byte* pointer
    ESocketReadStatus Read(Byte* buffer, Int32* size);

	void CloseSocket();

	void invalidateSocket();

	UInt16 getClientPort();
	const Byte* getClientIP();

    // Returns the status of a socket connection
    // 0 succeeded;
    // any other value is fail
    ESocketConnectionStatus startListenSocket();

private:
    Int32 translateAddress(UInt16 port, const Byte* pIp, sockaddr_in& sockaddress) const;
	// maximum buffer size for Read messages
	static const UInt32 s_MAX_BUFFER_SIZE = 128U;
    static const SOCKET InvalidSocket = -1;
	// client socket 
	SOCKET m_socket;
    // holds the parameters used to create this socket
    SSocketAttributes m_socketAttributes;

    struct sockaddr_in m_resolvedAddress;
};

inline void
CWinSocket::invalidateSocket()
{
	m_socket = INVALID_SOCKET;
	WSACleanup();
}

inline void
CWinSocket::CloseSocket()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}

inline UInt16
CWinSocket::getClientPort()
{
	return m_socketAttributes.m_port;
}

inline const Byte*
CWinSocket::getClientIP()
{
	return m_socketAttributes.m_ip.c_str();
}


#endif //_CWINSOCKET_H_
