#ifndef _CWINSOCKET_H_
#define _CWINSOCKET_H_

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <queue>
#include "CommonTypes.h"
#include "ISocket.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace Types;

class CWinSocket : public ISocket {
public:

	CWinSocket();
	~CWinSocket();
	// Returns the status of a socket connection
	// 0 succeeded;
	// any other value is fail
	ESocketConnectionStatus startListenSocket();
	
	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// content of buffer till it's size will be copied
	Int32 write(const Byte* buffer, Int32 const size);

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// the buffer will be copied to Byte* pointer
	Int32 read(Byte* buffer, Int32* size);

	void closeSocket();

	void invalidateSocket();

	UInt16 getClientPort();
	Byte* getClientIP();

private:
	// maximum buffer size for read messages
	static const UInt32 s_MAX_BUFFER_SIZE = 1024;
	// client socket 
	SOCKET m_clientSocket;
	// intentionally static
	// one listener shall be allocated per Port
	static SOCKET m_listenSocket;
	// details about this client socket 
	sockaddr_in m_clientInfo;

	// input buffer for commands
	char m_readBuffer[s_MAX_BUFFER_SIZE];
	// read buffer last index
	Int32 m_rbli;

};

inline void
CWinSocket::invalidateSocket()
{
	m_clientSocket = INVALID_SOCKET;
	WSACleanup();
}

inline void
CWinSocket::closeSocket()
{
	closesocket(m_clientSocket);
	m_clientSocket = INVALID_SOCKET;
}

inline UInt16
CWinSocket::getClientPort()
{
	return ntohs(m_clientInfo.sin_port);
}

inline Byte*
CWinSocket::getClientIP()
{
	return inet_ntoa(m_clientInfo.sin_addr);
}


#endif //_CWINSOCKET_H_
