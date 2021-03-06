#ifndef _IGAME_H_
#define _IGAME_H_

#include "CommonTypes.h"

class CGameController;

using namespace Types;

namespace UtilitiesCore
{
	class IClock;
}

namespace EngineCore
{

	class IMain
	{
	public:
		virtual void StartUp(int argv, char** argc) = 0;
	};

	class IGame
	{
	public:
		// Create needs to be implemented in the derived Game class
		static bool Create();
		// Should delete objects in the inverse order of their creation
		static void Destroy();
		virtual bool PreRendererInitialize(int argc, char* argv[]);
		virtual bool PostRendererInitialize();
		// a process to be executed in the background
		virtual void ExecuteBackground();

		virtual CGameController* GetGameController() = 0 ;

		void Display();
		void Reshape(Int32 w, Int32 h);
		// finalize this application
		inline void Quit() { mQuit = true; }
		// pauses the application without finalizing the application
		inline void Pause() { mPaused = true; }
		inline void Continue() { mPaused = false; }
		inline bool IsRunning() { return (!mPaused) && (!mQuit); }
		inline void IsReadyToClose() 
		{ 
			if (++mClosedThreads == 2)
				mReadyToClose = true; 
		}
		inline bool ReadyToClose() { return mReadyToClose; }
		// is fixed frames per second
		inline Int32 GetFps()
		{
			return mFps;
		}
		inline void SetFps(Int32 framesPerSecond)
		{
			mFps = framesPerSecond;
		}

		static IGame* mGame;          // global pointer

	protected:
		// constructor/destructor
		IGame();
		virtual ~IGame();

		bool ParseCommandLine(int argc, char* argv[]);
		bool SetupSubsystems(Types::EGraphicsAPI gfxApi);

		virtual void UpdateObjects(float dt) = 0;
		virtual void Render(float dt) = 0;

		// user has requested to quit
		bool mQuit;
		// only start closing the applications once we're ready to close
		bool mReadyToClose;
		bool mPaused;
		Int32 mFps;
		Int32 mClosedThreads;

		UtilitiesCore::IClock* mClock;

	private:
		// To avoid creation of this object without inheritance
		// copy operations
		IGame(const IGame& other);
		IGame& operator=(const IGame& other);
	};

}
#endif // _IGAME_H_