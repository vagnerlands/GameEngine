//===============================================================================
// @ IvVector4.h
// 
// 4D vector class
// ------------------------------------------------------------------------------
// Copyright (C) 2008 by Elsevier, Inc. All rights reserved.
//
//
//
//===============================================================================

#ifndef __IvPoint__h__
#define __IvPoint__h__

#include "CommonTypes.h"

using namespace Types;

class IvPoint
{
public:
	Int32 x;
	Int32 y;

	IvPoint() :
		x(0),
		y(0)
	{

	}

	IvPoint(Int32 argX, Int32 argY) :
		x(argX),
		y(argY)
	{

	}

	IvPoint& operator=(const IvPoint& arg)
	{
		x = arg.x;
		y = arg.y;
		return *this;
	}

	bool operator!=(const IvPoint& arg)
	{
		if ((arg.x == x) && (arg.y == y))
		{
			return false;
		}
		return true;
	}

	bool operator==(const IvPoint& arg)
	{
		if ((arg.x == x) && (arg.y == y))
		{
			return true;
		}
		return false;
	}
};

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------

#endif //__IvPoint__h__
