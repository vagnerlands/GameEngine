
#include "IGame.h"
#include "IRenderer.h"
#include "IClock.h"
#include "CClockOGL.h"
#include "GL/glut.h"

Int32 EngineCore::IGame::s_lastState = GLUT_UP;
Int32 EngineCore::IGame::s_lastCursorX = -1;
Int32 EngineCore::IGame::s_lastCursorY = -1;
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

void EngineCore::IGame::Update()
{
}

void EngineCore::IGame::Display()
{
	// render a scene according to current position of objects
	Render();
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
		mClock = new UtilitiesCore::CClockOGL();
	}
	else
	{
		return false;
	}
	// TODO: initialize threads
	// TODO: initialize mutexes/semaphores
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

	return true;
}

void
EngineCore::IGame::Reshape(Int32 w, Int32 h)
{
	Graphics::IRenderer::mRenderer->Resize(w, h);
}

void
EngineCore::IGame::RunWrap()
{
	// performs main logic features
	mGame->Display();

	if (mGame->mQuit)
	{
		mGame->Destroy();
	}
}

void
EngineCore::IGame::ReshapeWrap(Int32 w, Int32 h)
{
	mGame->Reshape(w, h);
}

// reads the user input key - KEY DOWN
void EngineCore::IGame::KeyboardInput(UByte key, Int32 x, Int32 y)
{
	if (key == 'c')
	{
		s_lastState = GLUT_DOWN;
		s_lastCursorX = -1;
		s_lastCursorY = -1;
	}
	//CGameCockpit::instance()->getGameController()->VOnKeyDown(key);
}

void EngineCore::IGame::MouseMotion(int x, int y)
{
	if ((s_lastCursorX != -1) && (s_lastCursorY != -1))
	{
		//CGameCockpit::instance()->getGameController()->VOnMouseMove(CPoint(x, y));
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

void EngineCore::IGame::MouseInput(Int32 button, Int32 state, Int32 x, Int32 y)
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
void EngineCore::IGame::KeyboardRelease(UByte key, int x, int y)
{

}