#ifndef _ISemaphore_H_
#define _ISemaphore_H_

#include <string>
#include <iostream>
#include "CommonTypes.h"

using namespace Types;
using namespace std;

//typedef TThreadEntryPoint PTHREAD_START_ROUTINE;

class ISemaphore 
{
public:
	virtual ~ISemaphore()
	{
		// empty - should be reinplemented by derived class
	}
	virtual void createSemaphore(string thName, Int32 count) = 0;
	virtual bool signal() = 0;
	virtual bool wait() = 0;
	virtual void destroy() = 0;
};

#endif
