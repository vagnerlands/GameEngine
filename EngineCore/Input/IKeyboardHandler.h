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

    struct KeyAttr
    {
        bool    isPressed = false;
        UInt32  cycles = 0; // cycles this key is being pressed
    };

	// Which keys are up and down
    KeyAttr     m_bKey[256];
};

#endif //_IKEYBOARDHANDLER_H_
