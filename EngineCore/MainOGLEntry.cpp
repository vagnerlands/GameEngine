// Game.cpp : Defines the entry point for the console application.
//

#include "Debugger.h"
#include "CRendererOGL.h"
#include "CThreadHolder.h"
#include "IvPoint.h"
#include "CGameController.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "IGame.h"

static Int32 s_lastState = GLUT_UP;
static Int32 s_lastCursorX = -1;
static Int32 s_lastCursorY = -1;

#ifdef WIN32
DWORD WINAPI BackgroundLoader(LPVOID lpParameter)
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

void Reshape(Int32 w, Int32 h)
{
	Graphics::IRenderer::mRenderer->Resize(w, h);
}

void RunWrap()
{
	// performs main logic features
	EngineCore::IGame::mGame->Display();
}

void ReshapeWrap(Int32 w, Int32 h)
{
	EngineCore::IGame::mGame->Reshape(w, h);
}

// reads the user input key - KEY DOWN
void KeyboardInput(UByte key, Int32 x, Int32 y)
{
	if (key == 'c')
	{
		s_lastState = GLUT_DOWN;
		s_lastCursorX = -1;
		s_lastCursorY = -1;
	}
	EngineCore::IGame::mGame->GetGameController()->VOnKeyDown(key);
}

void MouseMotion(int x, int y)
{
	if ((s_lastCursorX != -1) && (s_lastCursorY != -1))
	{
		EngineCore::IGame::mGame->GetGameController()->VOnMouseMove(IvPoint(x, y));
	}

	if (s_lastCursorX == -1)
	{
		s_lastCursorX = x;
	}

	if (s_lastCursorY == -1)
	{
		s_lastCursorY = y;
	}
}

void MouseInput(Int32 button, Int32 state, Int32 x, Int32 y)
{
	if (s_lastCursorX == -1)
	{
		s_lastCursorX = x;
	}

	if (s_lastCursorY == -1)
	{
		s_lastCursorY = y;
	}


	// Wheel reports as button 3(scroll up) and button 4(scroll down)
	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
		DEBUG_OUT("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
		if (button == 3)
		{
			//CGameCockpit::instance()->m_camera.MoveForward(1.0f);
		}
		else
		{
			//CGameCockpit::instance()->m_camera.MoveForward(-1.0f);
		}
	}
	else
	{  // normal button event
		DEBUG_OUT("Button %d - %s At %d %d\n", button, (state == GLUT_DOWN) ? "Down" : "Up", x, y);
	}

	s_lastCursorX = x;
	s_lastCursorY = y;
}

// reads the user input key - KEY UP
void KeyboardRelease(UByte key, int x, int y)
{
	EngineCore::IGame::mGame->GetGameController()->VOnKeyUp(key);
}

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

	// Attach callback functions to rendering functions
	// declares the drawing function to opengl state machine
	glutDisplayFunc(RunWrap);
	// declares the reshape function to opengl state machine
	glutReshapeFunc(ReshapeWrap);
	// declares the idle function to opengl state machine
	glutIdleFunc(RunWrap);
	// key press down event
	glutKeyboardFunc(KeyboardInput);
	// key press up event
	glutKeyboardUpFunc(KeyboardRelease);
	// mouse click input
	glutMouseFunc(MouseInput);
	// mouse motion input
	glutPassiveMotionFunc(MouseMotion);

#ifdef WIN32
	// creates a thread for loading resources purpose
	CThreadHolder::instance()->registerThread("thBackgroundLoader", BackgroundLoader);
#endif

	// initialize the opengl main loop
	// this will handle the whole displaying states of our game...
	glutMainLoop();

	// should never get to this point, but as a good practice...
	EngineCore::IGame::Destroy();

    return 0;
}

