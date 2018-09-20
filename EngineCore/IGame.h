#ifndef _IGAME_H_
#define _IGAME_H_

#include "CommonTypes.h"

using namespace Types;

namespace UtilitiesCore
{
	class IClock;
}

namespace EngineCore
{

	class IGame
	{
	public:
		// Create needs to be implemented in the derived Game class
		static bool Create();
		// Should delete objects in the inverse order of their creation
		static void Destroy();
		virtual bool PreRendererInitialize(int argc, char* argv[]);
		virtual bool PostRendererInitialize();

		void Update();
		void Display();
		void Reshape(Int32 w, Int32 h);
		inline void Quit() { mQuit = true; }
		inline bool IsRunning() { return !mQuit; }

		static IGame* mGame;          // global pointer

	protected:
		// rendering call back functions
		static void RunWrap();
		static void ReshapeWrap(Int32 w, Int32 h);
		static void KeyboardInput(UByte key, Int32 x, Int32 y);
		static void KeyboardRelease(UByte key, Int32 x, Int32 y);
		static void MouseInput(Int32 button, Int32 state, Int32 x, Int32 y);
		static void MouseMotion(Int32 x, Int32 y);
		// constructor/destructor
		IGame();
		virtual ~IGame();

		bool ParseCommandLine(int argc, char* argv[]);
		bool SetupSubsystems(Types::EGraphicsAPI gfxApi);

		virtual void UpdateObjects(float dt) = 0;
		virtual void Render() = 0;

		bool mQuit;
		bool mPaused;

		UtilitiesCore::IClock* mClock;

	private:
		// To avoid creation of this object without inheritance
		// copy operations
		IGame(const IGame& other);
		IGame& operator=(const IGame& other);

		static Int32 s_lastState;
		static Int32 s_lastCursorX;
		static Int32 s_lastCursorY;
	};

}
#endif // _IGAME_H_