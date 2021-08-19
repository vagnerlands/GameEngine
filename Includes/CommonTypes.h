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


template <typename Type> Type clamp(Type value, Type max) 
{
    if (value > max)
        return max;
    else
        return value;
}

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
    typedef unsigned long ULong;
    typedef long Long;

	static const Float s_PI = 3.14159265359F;
	static const Byte* s_GAME_CONNECTION_PORT = "1234";

	enum EGraphicsAPI
	{
		EGraphics_OGL,
		EGraphics_DirectX
	};

	enum CameraAttributeType
	{
		CameraAttribute_Position,
		CameraAttribute_Up,
		CameraAttribute_Right,
		CameraAttribute_Forward
	};

	// kind of cameras available, each one will give a different behavior for vector calculations
	enum ECameraType
	{
		// camera must be above ground and ignores roll rotation
		Camera_Human = 0,
		// good for third view games, camera is relative to a player position
		Camera_SpringArm,
		// camera may fly, but still ignores roll rotation
		Camera_Spectator,
		// free angle rotation
		Camera_Flight,
		// for iterators
		Camera_Total
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

    enum EApplicationMode
    {
        EAppMode_SENDER = 0,
        EAppMode_LISTENER,
        EAppMode_BOTH,
        EAppMode_INVALID
    };

    enum ESocketCreationStatus
    {
        ESOCKETCREATION_OK = 0,
        ESOCKETCREATION_FAIL,
        ESOCKETCREATION_TOTAL,
    };

    enum ESocketConnectionStatus
    {
        ESOCKETCONNECTION_OK = 0,
        ESOCKETCONNECTION_FAIL,
        ESOCKETCONNECTION_TOTAL,
    };


    enum ESocketReadStatus
    {
        ESOCKETREAD_OK = 0,
        ESOCKETREAD_FAIL,
        ESOCKETREAD_CONNECTION_LOST,
        ESOCKETREAD_TOTAL,
    };

    enum ESocketProtocol
    {
        SocketProtocol_UDP,
        SocketProtocol_TCP,
        SocketProtocol_Total
    };

    enum ESocketType
    {
        SocketType_STREAM,
        SocketType_DGRAM,
        SocketType_Total
    };

    struct SMulticastAttributes
    {
        //!< Loopback.
        UInt32 m_loopback;
        //!< TTL.
        UInt32 m_ttl;
        //!< Reuse.
        UInt32 m_reuse;
    };

    struct SSocketAttributes
    {
        // blocking type
        ULong m_isNonBlocking;
        // Is socket broadcasting (x.x.x.255)
        UInt32 m_isBroadcast;
        // timeout - in case of blocking
        UInt32 m_timeout;
        // Socket Type
        ESocketType m_type;
        // protocol
        ESocketProtocol m_protocol;
        // receive buffer size
        UInt32 m_receiveBufferSize;
        // send buffer size
        UInt32 m_sendingBufferSize;
        //!< IPv4 address - address from/to "where" this is being send
        string m_ip;
        //!< Network address.
        string m_netmask;
        //!< Interface IPv4 address - network adapter ip for routing purposes
        string m_interface;
        //!< Port (least significant byte first - host)
        UInt32 m_port;
        // multicast parameters
        SMulticastAttributes m_mc;
    };

	struct SRgba
	{
		UByte r;
		UByte g;
		UByte b;
		UByte a;
	};

	struct SRgb
	{
		UByte r;
		UByte g;
		UByte b;
	};

	enum eSceneItemType
	{
		eSceneItemType_Regular = 0, // only the most primitive commands required by the engine
		eSceneItemType_Simple, // no animations and no specific behaviors
		eSceneItemType_SkyBox, // To be used for skybox only
		eSceneItemType_Animated, // animated
		eSceneItemType_AnimatedAndShadowed, // animated and cast shadows
		eSceneItemType_NonAnimatedAndShadowed, // not animated, but cast shadows
		eSceneItemType_ParticlesSystem // animated according to different parameters
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


	static const UInt32 MAX_BONES = 100;
	
}

#endif //_COMMONTYPES_H_
