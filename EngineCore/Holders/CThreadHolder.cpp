#include "CThreadHolder.h"
#include "CWinThread.h"

CThreadHolder* CThreadHolder::s_pInstance = NULL;

CThreadHolder* 
CThreadHolder::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CThreadHolder();
	}

	return s_pInstance;
}

void 
CThreadHolder::destroyThread(string threadId)
{
	unordered_map<string, IThread*>::iterator threadInterface = m_threadMap.find(threadId);
	if (threadInterface != m_threadMap.end())
	{
		threadInterface->second->destroy();
		IThread* condemned = threadInterface->second;
		delete condemned;
	}
}

void CThreadHolder::registerThread(string thName, void * thEntry)
{
#ifdef WIN32
	IThread* thObj = new CWinThread();
#else
	// not implemented for other platforms
#endif
	if (thObj != NULL)
	{
		thObj->createThread(thName, thEntry);
	}
	else
	{
		cout << "Unable to allocate memory!" << endl;
	}

	m_threadMap.insert(make_pair(thName, thObj));

}

CThreadHolder::~CThreadHolder()
{
	delete s_pInstance;
}

CThreadHolder::CThreadHolder()
{
	// no implementation
}
