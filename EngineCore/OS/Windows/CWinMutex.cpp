#include "CWinMutex.h"

CWinMutex::CWinMutex()
{
}

CWinMutex::~CWinMutex()
{
}

void CWinMutex::createMutex(string mutexName)
{
	m_mutexName = mutexName;

	m_mutexHandle = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		(LPCWSTR)mutexName.data());             // unnamed mutex

	if (m_mutexHandle == NULL)
	{
		cout << "CreateMutex error: " << GetLastError() << endl;
	}
}

void
CWinMutex::mutexLock()
{
	DWORD dwWaitResult;
	// request the ownership of mutex...
	dwWaitResult = WaitForSingleObject(
		m_mutexHandle,   // handle to mutex
		10000L);   // five-second time-out interval
	switch (dwWaitResult)
	{
		// the thread got the mutex ownership...
	case WAIT_OBJECT_0:
		// mutex was performed
		break;
		// cannot get the mutex ownership due to time-out.
	case WAIT_TIMEOUT:
		cout << "time-out interval elapsed [" << m_mutexName << "], and the object's state is non-signaled." << endl;
		break;
		// got the ownership of the abandoned mutex object.
	case WAIT_ABANDONED:
		cout << "the mutex is set to non-signaled." << endl;
		break;
	}
}

void
CWinMutex::mutexUnlock()
{
	if (!ReleaseMutex(m_mutexHandle))
	{
		// deal with error.
		cout << "Error releasing mutex" << endl;
	}
}


void CWinMutex::destroy()
{
	CloseHandle(m_mutexHandle);
}
