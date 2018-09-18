#ifndef _COMMONTYPES_H_
#define _COMMONTYPES_H_

#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include <assert.h>

#include "Debugger.h"

class CResHandle;
using namespace std;

namespace Types 
{
	typedef unsigned int UInt32;
	typedef int Int32;
	typedef short Int16;
	typedef unsigned short UInt16;
	typedef char Byte;
	typedef unsigned char UByte;
	typedef double Double;
	typedef float Float;

	static const Float s_PI = 3.14159265359F;
	static const Byte* s_GAME_CONNECTION_PORT = "1234";

	enum CameraAttributeType
	{
		CameraAttribute_Position,
		CameraAttribute_Up,
		CameraAttribute_Right,
		CameraAttribute_Forward
	};

	enum GameViewType
	{
		GameView_Undefined,
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	enum ProcessType
	{
		ProcessType_LoadTexture,
		ProcessType_LoadModel,
		ProcessType_LoadShader
	};



	//typedef unsigned int GameViewId;

	/*
	class CPoint
	{
	public:
		TInt32 x;
		TInt32 y;

		CPoint() :
			x(0),
			y(0)
		{

		}

		CPoint(TInt32 argX, TInt32 argY) :
			x(argX),
			y(argY)
		{

		}

		CPoint& operator=(const CPoint& arg) 			
		{
			x = arg.x;
			y = arg.y;
			return *this;
		}

		bool operator!=(const CPoint& arg)
		{
			if ((arg.x == x) && (arg.y == y))
			{
				return false;
			}
			return true;
		}

		bool operator==(const CPoint& arg)
		{
			if ((arg.x == x) && (arg.y == y))
			{
				return true;
			}
			return false;
		}
	};


*/



	
	/*typedef unordered_map<string, shared_ptr<IViewElement>> ViewElementMap;
	typedef unordered_map<string, shared_ptr<IViewLight>> ViewLightMap;
	typedef std::list<shared_ptr<IView>> ViewList;
	typedef std::list<shared_ptr<IViewElement2D>> HUDList;*/

	typedef void(*OnRemoveEvent)(string);


	struct SFontAttributes
	{
		SFontAttributes(Int32 width, Int32 height, void* id) :
			m_fontWidth(width),
			m_fontHeight(height),
			m_fontId(id)
		{

		}
		Int32 m_fontWidth;
		Int32 m_fontHeight;
		void* m_fontId;
	};

	typedef unordered_map<string, SFontAttributes> FontMap;

	enum ESocketConnectionStatus
	{
		ESOCKETCONNECTION_OK = 0,
		ESOCKETCONNECTION_FAIL,
		ESOCKETCONNECTION_TOTAL,
	};

	//-------------------------------------------------------------------------------
	//-- Typedefs, Structs ----------------------------------------------------------
	//-------------------------------------------------------------------------------

	const char eol = '\n';
	typedef std::ostream IvWriter;

#ifndef ASSERT

#ifdef _DEBUG
#define ASSERT(x) \
    if (!(x)) \
	DEBUG_OUT( "Assertion failed: file " << __FILE__ << " line " << __LINE__ << std::endl ); \
    assert(x)
#else
#define ASSERT(x)
#endif

#endif

	
}

#endif //_COMMONTYPES_H_
