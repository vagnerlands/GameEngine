#include "CWinSemaphore.h"
#include <iostream>
using namespace std;

CWinSemaphore::~CWinSemaphore()
{

}

CWinSemaphore::CWinSemaphore()
{
	// empty implementation
}

void CWinSemaphore::createSemaphore(string thName, Int32 count)
{
    m_semaphoreHandle = CreateSemaphore(
        NULL,           // default security attributes
        0,  // initial count
        count,  // maximum count
        NULL);          // unnamed semaphore

    if (m_semaphoreHandle == NULL)
    {
        cout << "CreateSemaphore error: " << GetLastError() << endl;
        exit(1);
    }


}

bool CWinSemaphore::signal()
{
    bool ret = true;

    if (!ReleaseSemaphore(
        m_semaphoreHandle,  // handle to semaphore
        1,            // increase count by one
        NULL))       // not interested in previous count
    {
        cout << "ReleaseSemaphore error: " << GetLastError() << endl;
        ret = false;
    }

    return ret;
}

bool CWinSemaphore::wait()
{
    DWORD dwWaitResult = WaitForSingleObject(
        m_semaphoreHandle,   // handle to semaphore
        -1L);           // zero-second time-out interval

    return dwWaitResult == WAIT_OBJECT_0;
}


void CWinSemaphore::destroy()
{
	CloseHandle(m_semaphoreHandle);
}
