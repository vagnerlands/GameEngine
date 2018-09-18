#ifndef _CCLIENTSOCKET_H_
#define _CCLIENTSOCKET_H_

#include "CWinSocket.h"

#include "CommonTypes.h"
#include "CWinThread.h"
#include "ISocket.h"
#include "IMutex.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

class CSocketHolder 
{
public:
	CSocketHolder();
	~CSocketHolder();

	void addSocket(string user, ISocket* userSocket);
	void onConnectionEvent();
	bool dropSocket(string user);

	void readIncomingMsgs();

	void executeCommands();

	Int32 removeCondemedSockets();
private:
	// socket database - all client sockets shall be placed here
	// Key == client IP
	// Value == CWinSocket* object, including handle of the connection
	unordered_map<string, ISocket*> m_sockedDB;
	// mutex for m_socketDB
	IMutex* m_sockedDBMutex;
};

#endif //_CCLIENTSOCKET_H_
