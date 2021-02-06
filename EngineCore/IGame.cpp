
#include "IGame.h"
#include "IRenderer.h"
#include "IClock.h"
#include "CClockOGL.h"
#include "CThreadHolder.h"
#include "GL/glut.h"

EngineCore::IGame* EngineCore::IGame::mGame = nullptr;

void EngineCore::IGame::Destroy()
{
	if (mGame != nullptr)
		delete mGame;	
}

bool EngineCore::IGame::PreRendererInitialize(int argc, char * argv[])
{
	if (!ParseCommandLine(argc, argv))
	{ }
		

	if (!SetupSubsystems(Types::EGraphicsAPI::EGraphics_OGL))
	{ }

	return true;
}

bool EngineCore::IGame::PostRendererInitialize() 
{

	return true;
}

void EngineCore::IGame::ExecuteBackground()
{
	// this method should be overloaded
}


void EngineCore::IGame::Display()
{
	// checks whether the application is still running
	if (mGame->IsRunning())
	{
		// execute events and update databases
		mClock->StartFrame();
		// update all objects - the delta time (elapsed time)
		// is a good feature to increase intensity of movement 
		// if a few frames were lost - making the movements flow
		UpdateObjects(mClock->GetElapsedTime());
		// render a scene according to current position of objects
		Render(mClock->GetTimeInMili());

		mClock->Hold(mGame->GetFps());
	}

	if (ReadyToClose())
	{
		gDebugger.Flush();
		// kill all background threads
		CThreadHolder::instance()->DestroyAll();
		// release the rendering resources
		Graphics::IRenderer::Destroy();
		// release other game related resources
		EngineCore::IGame::Destroy();
		// exit with no error if get to this point
		exit(0);
	}

}

EngineCore::IGame::IGame() :
	mPaused(false), 
	mQuit(false),
	mReadyToClose(false),
	mFps(0),
	mClosedThreads(0)
{
	// 
}

EngineCore::IGame::~IGame()
{
	// TODO: destroy objects in the inverse order of the creation
}

bool EngineCore::IGame::ParseCommandLine(int argc, char * argv[])
{
	//TODO: parse the input commands
	return true;
}

bool EngineCore::IGame::SetupSubsystems(Types::EGraphicsAPI gfxApi)
{
	// initialize clock
	if (gfxApi == Types::EGraphicsAPI::EGraphics_OGL)
	{
		// Open GL implementation
		mClock = new UtilitiesCore::CClockOGL();
	}
	else
	{
		// no implementation found - recommend to quit this application in this case
		return false;
	}
	// TODO: initialize mutexes/semaphores

	return true;
}

void
EngineCore::IGame::Reshape(Int32 w, Int32 h)
{
	Graphics::IRenderer::mRenderer->Resize(w, h);
}