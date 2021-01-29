// Game.cpp : Defines the entry point for the console application.
//

#include "MainWinOgl.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "CGameController.h"

#include "CRendererOGL.h"
#include "CThreadHolder.h"
#include "IvPoint.h"

#include "MutexFactoryWin.h"
#include "SocketFactoryWin.h"
#include "ThreadFactoryWin.h"
#include "CFactory2dImageWin.h"


#ifdef WIN32
DWORD WINAPI WinOglBackgroundLoader(LPVOID lpParameter)
{
	// remains listening for more resource load requests
	while (EngineCore::IGame::mGame->IsRunning())
	{
		// load resources in background thread
		EngineCore::IGame::mGame->ExecuteBackground();
	}

	return (DWORD WINAPI)0;
}
#endif


void MainWinOgl::StartUp(int argv, char** argc)
{

	bool isFullScreen = false;
	if (isFullScreen)
	{

	}
	else
	{
		printf(" GPU & Driver: \n # %s \n # %s\n\n", glGetString(GL_VERSION), glGetString(GL_RENDERER));
	}
	// creates debug data file
	gDebugger.DumpToFile("Debug.txt");

	// Initialize all Factories
	MutexFactoryWin::Initialize();
	SocketFactoryWin::Initialize();
	ThreadFactoryWin::Initialize();
	//CFactory2dImageWin::Initialize();

	// set up game
	if (!EngineCore::IGame::Create() || !EngineCore::IGame::mGame->PreRendererInitialize(argv, argc))
	{
		EngineCore::IGame::Destroy();
		return;
	}

	if (!Graphics::CRendererOGL::Create() || !Graphics::IRenderer::mRenderer->InitializeGraphics(640, 480, false))
	{
		Graphics::CRendererOGL::Destroy();
		EngineCore::IGame::Destroy();
		return;
	}


	// Do post-renderer creation initialization
	if (!EngineCore::IGame::mGame->PostRendererInitialize())
	{
		Graphics::IRenderer::Destroy();
		EngineCore::IGame::Destroy();
		return;
	}

#ifdef WIN32
	// creates a thread for loading resources purpose
	CThreadHolder::instance()->registerThread("thBackgroundLoader", 0x03, WinOglBackgroundLoader);
#endif

	// should never get to this point, but as a good practice...
	//EngineCore::IGame::Destroy();

	return;
}

