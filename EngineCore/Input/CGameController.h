#ifndef _CGAMECONTROLLER_H_
#define _CGAMECONTROLLER_H_

#include "CommonTypes.h"
#include "IKeyboardHandler.h"
#include "IMouseHandler.h"
#include <string>

using namespace std;
using namespace Types;

#define KeyBindEvent(K) 			case K::cAscValue: \
									KeyBinderDelegator<K>::Instance().Event(); \
									break;

template <char KEY>
class IKey
{
public:
	static const Byte cAscValue = KEY;
};

class KeyBinderT : public IKey<116>
{
public:
	virtual void OnKeyEvent(const KeyBinderT& e) {};
};

class KeyBinderY : public IKey<121>
{
public:
	virtual void OnKeyEvent(const KeyBinderY& e) {};
};

class KeyBinderG : public IKey<103>
{
public:
	virtual void OnKeyEvent(const KeyBinderG& e) {};
};

class KeyBinderH : public IKey<104>
{
public:
	virtual void OnKeyEvent(const KeyBinderH& e) {};
};

template <typename KeyBinder>
class KeyBinderDelegator 
{
public:
	static KeyBinderDelegator& Instance()
	{
		static KeyBinderDelegator instance;
		return instance;
	}
	~KeyBinderDelegator() {}
	void BindKey(KeyBinder* b) { m_listeners.push_back(b); }
	void Event()
	{
		for (list<KeyBinder*>::iterator i = m_listeners.begin(); i != m_listeners.end(); i++)
		{
			(*i)->OnKeyEvent(KeyBinder());
		}
	}
private:
	list<KeyBinder*> m_listeners;
	KeyBinderDelegator()
	{

	}

};

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
			KeyBindEvent(KeyBinderT);
			KeyBindEvent(KeyBinderY);
			KeyBindEvent(KeyBinderG);
			KeyBindEvent(KeyBinderH);

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
