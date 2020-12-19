#ifndef _IKEYEVENT_H_
#define _IKEYEVENT_H_

#include "CommonTypes.h"

using namespace Types;

template <char KEY>
class IKey
{
public:
	virtual ~IKey() {}
	static const Byte cAscValue = KEY;
};

#endif //_IKEYEVENT_H_
