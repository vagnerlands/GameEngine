#ifndef _IGAME_H_
#define _IGAME_H_

#include "CommonTypes.h"
#include "CGameController.h"

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

    protected:
        // constructor/destructor
        IGame();
        virtual ~IGame();

        bool ParseCommandLine(int argc, char* argv[]);
        bool SetupSubsystems(Types::EGraphicsAPI gfxApi);

        virtual void UpdateObjects(float dt) = 0;
        virtual void Render(float dt);
        virtual void PostRender();

        // user has requested to quit
        bool mQuit;
        // only start closing the applications once we're ready to close
        bool mReadyToClose;
        bool mPaused;
        Int32 mMaxFps;
        Int32 mClosedThreads;

        std::shared_ptr<UtilitiesCore::IClock> mClock;
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
        Int32 GetFPS() const;


		// is fixed frames per second
		inline Int32 GetMaxFPS()
		{
			return mMaxFps;
		}
		inline void SetMaxFPS(Int32 framesPerSecond)
		{
			mMaxFps = framesPerSecond;
		}

		static IGame* mGame;          // global pointer
        CGameController mGameController;

	private:
		// To avoid creation of this object without inheritance
		// copy operations
		IGame(const IGame& other);
		IGame& operator=(const IGame& other);
	};

}
#endif // _IGAME_H_