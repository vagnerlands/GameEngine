#include "CThreadHolder.h"
#include "ThreadFactory.h"

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

	// creates a thread using the abstract factory
	IThread* thObj = ThreadFactory::Instance().Create(thName.data(), thEntry);

	if (thObj == nullptr)
	{
		cout << "Unable to allocate memory!" << endl;
	}
	else
	{
		m_threadMap.insert(make_pair(thName, thObj));
	}
}

void 
CThreadHolder::DestroyAll()
{
	// iterates through all objects and destroy one by one
	unordered_map<string, IThread*>::iterator threadObj = m_threadMap.begin();
	while (threadObj != m_threadMap.end())
	{
		threadObj->second->destroy();
		IThread* condemned = threadObj->second;
		delete condemned;
		threadObj++;
	}
	// clear
	m_threadMap = unordered_map<string, IThread*>();
}

CThreadHolder::~CThreadHolder()
{
	delete s_pInstance;
}

CThreadHolder::CThreadHolder()
{
	// no implementation
}
