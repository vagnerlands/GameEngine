#ifndef _CGAMECONTROLLER_H_
#define _CGAMECONTROLLER_H_
#include <iostream>
#include "CommonTypes.h"
#include "IKeyboardHandler.h"
#include "IMouseHandler.h"
#include "KeyDispatcher.h"
#include <string>

using namespace std;
using namespace Types;
using namespace _Keys;

#define KeyBindEvent(K) 			case Key##K##::cAscValue: \
											KeyBinderDispatcher<Key##K##>::Instance().Event(); \
											break;

class CGameController : public IKeyboardHandler, public IMouseHandler
{

public:

	// constructor
	CGameController(Int32 centerOfScreen_X, Int32 centerOfScreen_Y);
	// destructor
	virtual ~CGameController();

    void Resize(Int32 w, Int32 h);

	// this function is called cyclically and this updates the game status...
	void OnUpdate(float dt);

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
		m_bKey[c].isPressed = true; 
        ++m_bKey[c].cycles;
		return true; 
	}

	bool VOnKeyUp(const Byte c) 
	{
        m_bKey[c].cycles = 0;
		m_bKey[c].isPressed = false;
		switch (c) // fire event!
		{
            KeyBindEvent(0);
            KeyBindEvent(1);
            KeyBindEvent(2);
            KeyBindEvent(3);
            KeyBindEvent(4);
            KeyBindEvent(5);
            KeyBindEvent(6);
            KeyBindEvent(7);
            KeyBindEvent(8);
            KeyBindEvent(9);
            KeyBindEvent(q);
            KeyBindEvent(w);
            KeyBindEvent(e);
            KeyBindEvent(r);
            KeyBindEvent(t);
            KeyBindEvent(y);
            KeyBindEvent(u);
            KeyBindEvent(i);
            KeyBindEvent(o);
            KeyBindEvent(p);
            KeyBindEvent(a);
            KeyBindEvent(s);
            KeyBindEvent(d);
            KeyBindEvent(f);
            KeyBindEvent(g);
            KeyBindEvent(h);
            KeyBindEvent(j);
            KeyBindEvent(k);
            KeyBindEvent(l);
            KeyBindEvent(z);
            KeyBindEvent(x);
            KeyBindEvent(c);
            KeyBindEvent(v);
            KeyBindEvent(b);
            KeyBindEvent(n);
            KeyBindEvent(m);
            KeyBindEvent(Escape);

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
