// Game.cpp : Defines the entry point for the console application.
//

#include "Debugger.h"
#include "CRendererOGL.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "IGame.h"

int 
main(int argv, char** argc)
{
	// Use a single buffered window in RGB mode (as opposed to a double-buffered
	// window or color-index mode).
	glutInit(&argv, argc);
	// only one buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Position window at (80,80) and sets to it a title
	glutInitWindowPosition(200, 20);
	bool isFullScreen = false;
	if (isFullScreen)
	{
		glutGameModeString("640x480:16@60");
		glutEnterGameMode();
	}
	else
	{
		glutInitWindowSize(640, 480);
		glutCreateWindow("EngineCore - Vagner Landskron");

		printf(" GPU & Driver: \n # %s \n # %s\n\n",glGetString(GL_VERSION), glGetString(GL_RENDERER));
	}
	// creates debug data file
	gDebugger.DumpToFile("Debug.txt");

	// set up game
	if (!EngineCore::IGame::Create() || !EngineCore::IGame::mGame->PreRendererInitialize(argv, argc))
	{
		EngineCore::IGame::Destroy();
		return 1;
	}

	if (!Graphics::CRendererOGL::Create() || !Graphics::IRenderer::mRenderer->InitializeGraphics(640, 480, false))
	{
		Graphics::CRendererOGL::Destroy();
		EngineCore::IGame::Destroy();
		return 1;
	}


	// Do post-renderer creation initialization
	if (!EngineCore::IGame::mGame->PostRendererInitialize())
	{
		Graphics::IRenderer::Destroy();
		EngineCore::IGame::Destroy();
		return 1;
	}

	// initialize the opengl main loop
	// this will handle the whole displaying states of our game...
	glutMainLoop();

	// should never get to this point, but as a good practice...
	EngineCore::IGame::Destroy();

    return 0;
}

