#include "CWinSocket.h"

SOCKET CWinSocket::m_listenSocket = INVALID_SOCKET;

CWinSocket::CWinSocket()
{
	m_clientSocket = INVALID_SOCKET;
	m_recycle = false;
}

CWinSocket::~CWinSocket()
{
	// temporary variables
	int iResult;
	// shutdown the connection since we're done
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	} 
	else
	{
		// cleanup
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}
}

ESocketConnectionStatus
CWinSocket::startListenSocket()
{

	// temporary data for connection
	// -----
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	WSADATA wsaData;
	int iResult;
	// -----

	if (m_listenSocket == INVALID_SOCKET)
	{
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) 
		{
			printf("WSAStartup failed with error: %d\n", iResult);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		// zeroise this address of hints till the size of hints
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, (PCSTR)Types::s_GAME_CONNECTION_PORT, &hints, &result);
		if (iResult != 0) 
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		// blocking operation - make sure this is being called in a thread
		m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		// Setup the TCP listening socket
		iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(m_listenSocket);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		freeaddrinfo(result);
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		invalidateSocket();
		return ESOCKETCONNECTION_FAIL;
	}

	// Accept a client socket
	int size = sizeof(m_clientInfo);
	m_clientSocket = accept(m_listenSocket, (sockaddr*)&m_clientInfo, &size);
	if (m_clientSocket == INVALID_SOCKET) 
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		invalidateSocket();
		return ESOCKETCONNECTION_FAIL;
	}

	// non-blocking client socket
	u_long iMode = 1;
	ioctlsocket(m_clientSocket, FIONBIO, &iMode);

	return ESOCKETCONNECTION_OK;

}

Int32 
CWinSocket::write(const Byte* buffer, Int32 const size)
{
	// temporary variable
	int iSendResult;
	// Echo the buffer back to the sender
	iSendResult = send(m_clientSocket, buffer, size, 0);
	if (iSendResult == SOCKET_ERROR) 
	{
		printf("Connection was lost: %d\n", WSAGetLastError());
		closesocket(m_clientSocket);
		// error
		return 1;
	}
	// message sent
	return 0;
}


// incoming buffer
// size in bytes
Int32
CWinSocket::read(Byte* buffer, Int32 *size)
{
	// read the socket and place content in buffer*
	// *size shall hold the amount of bytes read
	*size = recv(m_clientSocket, buffer, strlen(buffer), 0);
	if (*size > 0)
	{
		if ((m_rbli + *size) < (s_MAX_BUFFER_SIZE - 1))
		{
			memcpy(&m_readBuffer[m_rbli], buffer, *size);
			m_rbli += *size;
			m_readBuffer[m_rbli + 1] = 0;
		}

		return 0;
	}
	else if ((*size == 0) || (m_clientSocket == INVALID_SOCKET))
	{
		printf("Connection was closed\n");
		return 100;
	} 
	else
	{
		// No data available
	}
	

	// message sent
	return 0;
}

