#ifndef _CWINTHREAD_H_
#define _CWINTHREAD_H_

#include <Windows.h>
#include <string>
#include <iostream>
#include "CommonTypes.h"
#include "IThread.h"

using namespace Types;
using namespace std;

class CWinThread : public IThread 
{
public:
	CWinThread();
	~CWinThread();
	void createThread(string thName, Byte affinity, void* thEntry) override;
	void destroy() override;
private:
	DWORD m_threadID;
	HANDLE m_threadHandle;
};

#endif
