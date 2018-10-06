
#include "IGame.h"
#include "IRenderer.h"
#include "IClock.h"
#include "CClockOGL.h"
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
		return false;

	if (!SetupSubsystems(Types::EGraphicsAPI::EGraphics_OGL))
		return false;

	return true;
}

bool EngineCore::IGame::PostRendererInitialize() 
{
	// TODO: threads, semaphores, sockets and other features that are better to be initialized
	// at the end of the initialization process...
	return true;
}

void EngineCore::IGame::ExecuteBackground()
{
	// do something here

	// testing it
	static int vagner = 0;
	vagner++;
}

void EngineCore::IGame::Update()
{
	// checks whether the application is still running
	if (mGame->IsRunning())
	{
		// execute events and update databases
	}
}

void EngineCore::IGame::Display()
{
	// checks whether the application is still running
	if (mGame->IsRunning())
	{
		// render a scene according to current position of objects
		Render();

		if (mQuit)
		{
			mGame->Destroy();
		}
	}
}

EngineCore::IGame::IGame() :
	mPaused(false), 
	mQuit(false)
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