#ifndef _CTHREADHOLDER_H_
#define _CTHREADHOLDER_H_

#include "CommonTypes.h"
#include "IThread.h"

#include <string>
#include <iostream>
#include <unordered_map>

using namespace Types;
using namespace std;

class CThreadHolder 
{
public:
	static CThreadHolder* instance();
	void destroyThread(string threadId);
	void registerThread(string thName, Byte affinity, void* thEntry);
	void DestroyAll();
private:
	CThreadHolder();
	~CThreadHolder();
	static CThreadHolder* s_pInstance;

	unordered_map<string, IThread*> m_threadMap;
};

#endif //_CTHREADHOLDER_H_
