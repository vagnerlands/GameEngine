#ifndef _IMUTEX_H_
#define _IMUTEX_H_

#include <string>
#include <iostream>
#include "CommonTypes.h"

using namespace Types;
using namespace std;

// to be used on shared resources and avoid racing situations
class IMutex {
public:
	virtual ~IMutex()
	{
		// empty - should be reinplemented by derived class
	}
	virtual void createMutex(string mutexName) = 0;
	virtual void mutexLock() = 0;
	virtual void mutexUnlock() = 0;
	virtual void destroy() = 0;
};

#endif //_IMUTEX_H_
