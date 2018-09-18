#ifndef _CWINMUTEX_H_
#define _CWINMUTEX_H_

#include <Windows.h>
#include <string>
#include <iostream>
#include "CommonTypes.h"
#include "IMutex.h"

using namespace Types;
using namespace std;

class CWinMutex : public IMutex {
public:
	CWinMutex();
	~CWinMutex();
	//void createThread(string thName, LPTHREAD_START_ROUTINE thEntry);
	void createMutex(string mutexName);
	void mutexLock();
	void mutexUnlock();
	void destroy();
private:
	HANDLE m_mutexHandle;
	UInt32 m_counter;
	string m_mutexName;
};

#endif //_CWINMUTEX_H_
