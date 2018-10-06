#ifndef _IKEYBOARDHANDLER_H_
#define _IKEYBOARDHANDLER_H_

#include "CommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IKeyboardHandler 
{
public:
	virtual ~IKeyboardHandler()
	{
		// no implementation
	}

	virtual bool VOnKeyDown(const Byte c) = 0;

	virtual bool VOnKeyUp(const Byte c) = 0;

	// Which keys are up and down
	bool m_bKey[256];
};

#endif //_IKEYBOARDHANDLER_H_
