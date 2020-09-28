// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "GL/freeglut.h"
#include <stdio.h>
#include <iostream>

#include "Game.h"
#include "CGameController.h"
#include "MainWinOgl.h"
#include "OS/Abstract/ThreadFactory.h"
#include "OS/Abstract/IThread.h"

static int s_WIDTH = 640;
static int s_HEIGHT = 480;
static int s_MOUSE_X = 320;
static int s_MOUSE_Y = 240;
static int s_lastState = 1;
static int s_lastCursorX = -1;
static int s_lastCursorY = -1;

static float s_R = 1.f, s_G = 0.f, s_B = 0.f;

HGLRC           hRC = NULL;                           // Permanent Rendering Context
HDC             hDC = NULL;                           // Private GDI Device Context
HWND            hWnd = NULL;                          // Holds Our Window Handle
HINSTANCE       hInstance;                          // Holds The Instance Of The Application 

bool    keys[256];                              // Array Used For The Keyboard Routine
bool    active = TRUE;                                // Window Active Flag Set To TRUE By Default
bool    fullscreen = TRUE;                            // Fullscreen Flag Set To Fullscreen Mode By 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc
int StartUp();

extern "C" __declspec(dllexport)  void
__stdcall SetWndParams(int width, int height)
{
	s_WIDTH = width;
	s_HEIGHT = height;
}


extern "C" __declspec(dllexport)  void
__stdcall SetWndHnd(HWND renderWindow)
{
	hWnd = renderWindow;
    // creates a thread, since this StartUp is a forever running blocking operation
    DWORD thId;
    HANDLE thHnd = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartUp, 0, 0x00000000, &thId);
    if (thHnd == NULL)
    {
        cout << "CreateThread error: " << GetLastError() << " on creating <GameEditorTh>" << endl;
    }
}

extern "C" __declspec(dllexport)  void
__stdcall SetRenderColor(float r, float g, float b)
{
	s_R = r;
	s_B = b;
	s_G = g;
}

// TODO: Create the following interfaces
// 1. Mouse motion (sending relative coordinate according to canvas size);
// 2. Pressed key
// 3. Released key
// 4. Resize viewport

extern "C" __declspec(dllexport)  void
__stdcall SetMousePosition(int x, int y)
{
	if ((s_lastCursorX != -1) && (s_lastCursorY != -1))
	{
		Game::mGame->GetGameController()->VOnMouseMove(IvPoint(x, y));
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

extern "C" __declspec(dllexport)  void
__stdcall SetKeyPressed(int keyId)
{
	keys[keyId] = true;
	Game::mGame->GetGameController()->VOnKeyDown(keyId);
}

extern "C" __declspec(dllexport)  void
__stdcall SetKeyReleased(int keyId)
{
	keys[keyId] = false;
	Game::mGame->GetGameController()->VOnKeyUp(keyId);
}

extern "C" __declspec(dllexport)  void
__stdcall UpdateViewport(int width, int height)
{
	s_WIDTH = width;
	s_HEIGHT = height;

	Game::mGame->Reshape(width, height);
}

extern "C" __declspec(dllexport)  void
__stdcall SetMouseAction(int x, int y, int button, int state)
{
	if (s_lastCursorX == -1)
	{
		s_lastCursorX = x;
	}

	if (s_lastCursorY == -1)
	{
		s_lastCursorY = y;
	}

	s_lastCursorX = x;
	s_lastCursorY = y;

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

		if (button == 0) // It's a wheel event
		{
			if (state == GLUT_DOWN)
				Game::mGame->GetGameController()->VOnLButtonDown(IvPoint(x, y));
			else
				Game::mGame->GetGameController()->VOnLButtonUp(IvPoint(x, y));
		}
		else if (button == 2)
		{
			if (state == GLUT_DOWN)
				Game::mGame->GetGameController()->VOnRButtonDown(IvPoint(x, y));
			else
				Game::mGame->GetGameController()->VOnRButtonUp(IvPoint(x, y));
		}
	}
    
}


int InitGL(GLvoid)                              // All Setup For OpenGL Goes Here

{
	glShadeModel(GL_SMOOTH);                        // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background

	glClearDepth(1.0f);                         // Depth Buffer Setup

	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing

	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do

	return TRUE;                                // Initialization Went OK

}


GLvoid KillGLWindow(GLvoid)                         // Properly Kill The Window

{


	if (fullscreen)                             // Are We In Fullscreen Mode?

	{
		ChangeDisplaySettings(NULL, 0);                  // If So Switch Back To The Desktop

		ShowCursor(TRUE);                       // Show Mouse Pointer

	}






	if (hRC)                                // Do We Have A Rendering Context?

	{



		if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?

		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}

		if (!wglDeleteContext(hRC))                 // Are We Able To Delete The RC?

		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}

		hRC = NULL;                           // Set RC To NULL

	}

	if (hDC && !ReleaseDC(hWnd, hDC))                    // Are We Able To Release The DC

	{

		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hDC = NULL;                           // Set DC To NULL

	}
	if (!UnregisterClass("OpenGL", hInstance))               // Are We Able To Unregister Class

	{

		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hInstance = NULL;                         // Set hInstance To NULL

	}

}



BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)

{
	GLuint      PixelFormat;                        // Holds The Results After Searching For A Match
	WNDCLASS    wc;                         // Windows Class Structure
	DWORD       dwExStyle;                      // Window Extended Style
	DWORD       dwStyle;                        // Window Style

	RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;                        // Set Left Value To 0
	WindowRect.right = (long)width;                       // Set Right Value To Requested Width
	WindowRect.top = (long)0;                         // Set Top Value To 0
	WindowRect.bottom = (long)height;                     // Set Bottom Value To Requested Height
	fullscreen = fullscreenflag;                      // Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = (WNDPROC)WndProc;                // WndProc Handles Messages
	wc.cbClsExtra = 0;                        // No Extra Window Data
	wc.cbWndExtra = 0;                        // No Extra Window Data
	wc.hInstance = hInstance;                    // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
	wc.hbrBackground = NULL;                     // No Background Required For GL
	wc.lpszMenuName = NULL;                     // We Don't Want A Menu
	wc.lpszClassName = "OpenGL";                 // Set The Class Name
	if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Exit And Return FALSE
	}

	if (fullscreen)                             // Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;                   // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;            // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;           // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;             // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "Game-Editor", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;                   // Exit And Return FALSE
			}
		}
	}

	if (fullscreen)                             // Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;                  // Window Extended Style
		dwStyle = WS_POPUP;                       // Windows Style
		ShowCursor(FALSE);                      // Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

																	/*if (!(hWnd = CreateWindowEx(dwExStyle,              // Extended Style For The Window
																	"OpenGL",               // Class Name
																	title,                  // Window Title
																	WS_CLIPSIBLINGS |           // Required Window Style
																	WS_CLIPCHILDREN |           // Required Window Style
																	dwStyle,                // Selected Window Style
																	0, 0,                   // Window Position
																	WindowRect.right - WindowRect.left,   // Calculate Adjusted Window Width
																	WindowRect.bottom - WindowRect.top,   // Calculate Adjusted Window Height
																	NULL,                   // No Parent Window
																	NULL,                   // No Menu
																	hInstance,              // Instance
																	NULL)))                 // Don't Pass Anything To WM_CREATE
																	{
																	KillGLWindow();                         // Reset The Display
																	MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
																	return FALSE;                           // Return FALSE
																	}*/

	static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		bits,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		24,                             // 16Bit Z-Buffer (Depth Buffer)
		8,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                         // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                             // Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))                         // Did We Get A Device Context?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))             // Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))                   // Are We Able To Get A Rendering Context?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))                        // Try To Activate The Rendering Context
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);                       // Show The Window
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);                             // Sets Keyboard Focus To The Window
	//ReSizeGLScene(width, height);                       // Set Up Our Perspective GL Screen

	if (!InitGL())                              // Initialize Our Newly Created GL Window
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	return TRUE;                                // Success
}


LRESULT CALLBACK WndProc(HWND    hWnd,                   // Handle For This Window
	UINT    uMsg,                   // Message For This Window
	WPARAM  wParam,                 // Additional Message Information
	LPARAM  lParam)                 // Additional Message Information
{

	switch (uMsg)                               // Check For Windows Messages

	{

	case WM_ACTIVATE:                       // Watch For Window Activate Message

	{

		if (!HIWORD(wParam))                    // Check Minimization State

		{

			active = TRUE;                    // Program Is Active

		}

		else

		{

			active = FALSE;                   // Program Is No Longer Active

		}



		return 0;                       // Return To The Message Loop

	}

	case WM_SYSCOMMAND:                     // Intercept System Commands

	{

		switch (wParam)                     // Check System Calls

		{

		case SC_SCREENSAVE:             // Screensaver Trying To Start?

		case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?

			return 0;                   // Prevent From Happening

		}

		break;                          // Exit

	}

	case WM_CLOSE:                          // Did We Receive A Close Message?

	{

		PostQuitMessage(0);                 // Send A Quit Message

		return 0;                       // Jump Back

	}

	case WM_KEYDOWN:                        // Is A Key Being Held Down?

	{

		keys[wParam] = TRUE;                    // If So, Mark It As TRUE

		return 0;                       // Jump Back

	}

	case WM_KEYUP:                          // Has A Key Been Released?

	{

		keys[wParam] = FALSE;                   // If So, Mark It As FALSE

		return 0;                       // Jump Back

	}

	case WM_SIZE:                           // Resize The OpenGL Window

	{

		//ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));       // LoWord=Width, HiWord=Height

		return 0;                       // Jump Back

	}

	}
	// Pass All Unhandled Messages To DefWindowProc

	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

int StartUp()
{
	// Windows Message Structure
	MSG msg;                                
	// Bool Variable To Exit Loop
	BOOL done = FALSE;                         
	// Windowed Mode		
	fullscreen = FALSE;                       
											  
												  
	// Create Our OpenGL Window
	if (!CreateGLWindow("Game-Editor", s_WIDTH, s_HEIGHT, 32, fullscreen))
	{
		return 0;                           // Quit If Window Was Not Created
	}

	// Must be instantiated after the window assignment
	MainWinOgl* obj = new MainWinOgl();
	// check for validity
	if (obj == nullptr)
	{
		MessageBox(NULL, "No Memory available to start the Editor", "Closing Editor", MB_OK | MB_ICONINFORMATION);
		exit(1);
	}
	// start-up all engine mechanisms
	obj->StartUp(0, 0);

    Game::mGame->Reshape(s_WIDTH, s_HEIGHT);

	// Loop That Runs Until done=TRUE
	while (!done)                                
	{
		// Is There A Message Waiting? 
		// these messages are user inputs on the application, generally
		// or messages from one application to this application
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))           
		{
			// Have We Received A Quit Message?
			if (msg.message == WM_QUIT)               
			{
				// If So done=TRUE
				done = TRUE;                  
			}
			// If Not, Deal With Window Messages
			else                            
			{
				// Translate The Message
				TranslateMessage(&msg);             
				// Dispatch The Message
				DispatchMessage(&msg);              
			}
		}
		// If There Are No Messages
		else                                
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			// Program Active?
			if (active)                     
			{	
				// Was ESC Pressed?
				if (keys[VK_ESCAPE])                
				{
					// ESC Signalled A Quit
					done = TRUE;              
				}
				// Not Time To Quit, Update Screen
				else                        
				{
					// Draw The Scene
					Game::mGame->Display();     
					// Swap Buffers (Double Buffering)
					SwapBuffers(hDC);           
				}
			}
			// Is F1 Being Pressed?
			if (keys[VK_F1])                    
			{
				// If So Make Key FALSE
				keys[VK_F1] = FALSE;            
				// Kill Our Current Window
				KillGLWindow();                 
				// Toggle Fullscreen / Windowed Mode
				fullscreen = !fullscreen;             
				// Recreate Our OpenGL Window													  
				if (!CreateGLWindow("Game-Editor", 640, 480, 16, fullscreen))
				{
					// Quit If Window Was Not Created
					return 0;               
				}
			}
		}
	}
	// Shutdown routine

	// Kill The Window
	KillGLWindow();                             
	// Exit The Program
	return (msg.wParam);                            
}
