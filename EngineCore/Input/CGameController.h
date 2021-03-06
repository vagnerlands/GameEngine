#ifndef _CGAMECONTROLLER_H_
#define _CGAMECONTROLLER_H_

#include "CommonTypes.h"
#include "IKeyboardHandler.h"
#include "IMouseHandler.h"
#include "Keys.h"
#include <string>

using namespace std;
using namespace Types;

#define KeyBindEvent(K) 			case K::cAscValue: \
											_Keys::KeyBinderDispatcher<K>::Instance().Event(); \
											break;

class CGameController : public IKeyboardHandler, public IMouseHandler
{

public:

	// constructor
	CGameController(Int32 centerOfScreen_X, Int32 centerOfScreen_Y);
	// destructor
	virtual ~CGameController();

	// this function is called cyclically and this updates the game status...
	void OnUpdate();

	virtual bool VOnMouseMove(const IvPoint &mousePos);

	virtual bool VOnLButtonDown(const IvPoint &mousePos)
	{
		m_isLeftButtonPressed = true;
		return true;
	}

	virtual bool VOnLButtonUp(const IvPoint &mousePos)
	{
		m_isLeftButtonPressed = false;
		return true;
	}

	virtual bool VOnRButtonDown(const IvPoint &mousePos)
	{
		m_isRightButtonPressed = true;
		return true;
	}

	virtual bool VOnRButtonUp(const IvPoint &mousePos)
	{
		m_isRightButtonPressed = false;
		return true;
	}

	bool VOnKeyDown(const Byte c) 
	{ 
		m_bKey[c] = true; 
		return true; 
	}

	bool VOnKeyUp(const Byte c) 
	{ 
		m_bKey[c] = false; 
		switch (c)
		{
			KeyBindEvent(_Keys::Key0);
			KeyBindEvent(_Keys::Key1);
			KeyBindEvent(_Keys::Key2);
			KeyBindEvent(_Keys::Key3);
			KeyBindEvent(_Keys::KeyT);
			KeyBindEvent(_Keys::KeyY);
			KeyBindEvent(_Keys::KeyG);
			KeyBindEvent(_Keys::KeyH);

			default:
				break;
		}
		return true; 
	}

private:
	// only used internally - center of screen on X axis
	Int32 mCenterOfScreen_X;
	// only used internally - center of screen on Y axis
	Int32 mCenterOfScreen_Y;

};

#endif //_CGAMECONTROLLER_H_
