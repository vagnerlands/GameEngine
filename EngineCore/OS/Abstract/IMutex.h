#ifndef _IMUTEX_H_
#define _IMUTEX_H_

#include "CommonTypes.h"

using namespace Types;

// to be used on shared resources and avoid racing situations
class IMutex {
public:
	virtual ~IMutex()
	{
		// empty - should be reinplemented by derived class
	}
	virtual void createMutex(const Byte* mutexName) = 0;
	virtual void mutexLock() = 0;
	virtual void mutexUnlock() = 0;
	virtual void destroy() = 0;
};

class LockGuard
{
public:
	explicit LockGuard(class IMutex* pMutex) : m_pMutex(pMutex) { m_pMutex->mutexLock(); }
	~LockGuard() { m_pMutex->mutexUnlock(); }
private:
	mutable class IMutex* m_pMutex;
};

#endif //_IMUTEX_H_
