#ifndef _IMOUSEHANDLER_H_
#define _IMOUSEHANDLER_H_

#include "CommonTypes.h"
#include "IvPoint.h"
#include <string>

using namespace std;
using namespace Types;

class IMouseHandler
{
public:

	virtual ~IMouseHandler()
	{

	}
	
	virtual bool VOnMouseMove(const IvPoint& mousePos) = 0;
	virtual bool VOnLButtonDown(const IvPoint& mousePos) = 0;
	virtual bool VOnLButtonUp(const IvPoint& mousePos) = 0;
	virtual bool VOnRButtonDown(const IvPoint& mousePos) = 0;
	virtual bool VOnRButtonUp(const IvPoint& mousePos) = 0;
	
	// attributes about mouse movement for decisions
	Int32 m_movementIntensityX;
	Int32 m_movementIntensityY;
	Int32 m_movementAngle;
	Int32 m_distanceFromCenter;
	Int32 m_angleFromCenter;

	bool m_isLeftButtonPressed;
	bool m_isRightButtonPressed;

	IvPoint m_lastPos;
	
};

#endif //_IMOUSEHANDLER_H_
