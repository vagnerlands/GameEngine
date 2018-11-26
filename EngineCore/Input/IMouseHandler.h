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
	IMouseHandler() :
		m_movementIntensityX(0),
	    m_movementIntensityY(0),
		m_movementAngle(0),
		m_distanceFromCenter(0),
		m_angleFromCenter(0),
		m_isLeftButtonPressed(false),
		m_isRightButtonPressed(false)
	{

	}
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
