#ifndef _IKEYEVENT_H_
#define _IKEYEVENT_H_

#include "CommonTypes.h"

using namespace Types;

template <UByte KEY>
class IKey
{
public:
	virtual ~IKey() {}
	static const UByte cAscValue = KEY;
};

#endif //_IKEYEVENT_H_
