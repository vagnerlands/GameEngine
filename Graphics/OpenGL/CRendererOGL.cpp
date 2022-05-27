#include "CRendererOGL.h"
#include "TextRendererOGL.h"
#include "IvMath.h"
#include "IvMatrix33.h"
#include "GL/freeglut.h"
#include <iostream>

#ifdef _IS_EDITOR_
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#include "imgui.h"
#include "imgui/res/bindings/imgui_impl_glut.h"
#include "imgui/res/bindings/imgui_impl_opengl3.h"
#endif

// Our state
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


bool Graphics::CRendererOGL::Create()
{
	if (mRenderer == nullptr)
	{
		mRenderer = new CRendererOGL();
        TextRendererOGL::Create();
	}
	return (mRenderer != nullptr);
}

void Graphics::CRendererOGL::Destroy()
{
	if (mRenderer != nullptr)
	{
		delete mRenderer;
	}

#ifdef _IS_EDITOR_
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
#endif

	mRenderer = 0;
}

void Graphics::CRendererOGL::PrepareFrame()
{
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);

#ifdef _IS_EDITOR_
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();
#endif

#ifdef _IS_EDITOR_
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}


	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();

#endif

}
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/glm.hpp>
void Graphics::CRendererOGL::PrepareCamera2D()
{
	GLenum glErr = 0;
    glClear(GL_DEPTH_BUFFER_BIT); // avoid 2d objects to be "cut" by 3d objects in the scene

	IvMatrix44 ortho;

    const Float cNear   = -1.0;
    const Float cFar    =  1.0;

	Float recipX = (Float)(mWidth);
	Float recipY = (Float)(mHeight);
	Float recipZ = (Float)(cFar - cNear);


	ortho(0, 0) = 2.0f/recipX;
    ortho(1, 1) = 2.0f/recipY;
    ortho(2, 2) = 1.f; //2.0f/recipZ;

	ortho(0, 3) = -(mWidth/recipX);
	ortho(1, 3) = -(mHeight/recipY);
	//ortho(2, 3) =  ((cFar + cNear)/recipZ);

	glErr = glGetError();

    SetProjectionMatrix(ortho);    
	glErr = glGetError();
}

void Graphics::CRendererOGL::PrepareCamera3D()
{
	// TODO: this shall change only when window changes
	// makes another function calculate the "perspective"
	GLenum glErr = 0;
	while ((glErr = glGetError()) != GL_NO_ERROR)
		continue;
		//printf("Cleaning errors at CRendererGLFW::PrepareCamera3D %d\n", glErr);
	// clear the depth buffer and the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set up current viewport based on defined screen size
	glViewport(0, 0, mWidth, mHeight);
	// d is distance from view view position to the projection plane
	float d = 1.0f / IvTan(mFOV / 180.0f * Types::s_PI * 0.5f);
	// a normalized 
	float recip = 1.0f / (mNear - mFar);
	IvMatrix44 perspective;

	perspective(0, 0) = d / ((GLfloat)mWidth / (GLfloat)mHeight);
	perspective(1, 1) = d;
	perspective(2, 2) = (mNear + mFar)*recip;
	perspective(2, 3) = 2 * mNear*mFar*recip;
	perspective(3, 2) = -1.0f;
	perspective(3, 3) = 0.0f;

	SetProjectionMatrix(perspective);

	IvMatrix44 ident;
	SetViewMatrix(ident);
	SetWorldMatrix(ident);

	
	// set default modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	// TODO: initOrient won't change - make this variable static and initialized
	// somewhere else

	IvMatrix33 initOrient;
	initOrient.Identity();

	IvMatrix33 viewToWorldRot = mCamera.m_camRotation*initOrient;

	// set view matrix

	// world to view rotation is transpose of view to world
	IvMatrix44 matrix(::Transpose(viewToWorldRot));
	// world to view translation is -eye position times world to view rotation
	// or can multiply by transpose of view to world
	IvVector3 eyeInverse = -mCamera.m_position*viewToWorldRot;
	// set translation of matrix
	matrix(0, 3) = eyeInverse.GetX();
	matrix(1, 3) = eyeInverse.GetY();
	matrix(2, 3) = eyeInverse.GetZ();
	SetViewMatrix(matrix);
    mFrustum.CalculateFrustum();
}

void Graphics::CRendererOGL::SwapBuffer()
{
	GLenum err = GL_NO_ERROR;
#ifdef _IS_EDITOR_
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
#endif

    glutSwapBuffers();
}

//-------------------------------------------------------------------------------
// @ CRendererOGL::Resize()
//-------------------------------------------------------------------------------
// Set up GL parameters for window size
//-------------------------------------------------------------------------------
void
	Graphics::CRendererOGL::Resize(UInt32 width, UInt32 height)
{
	// prevent divide by zero
	if (height == 0)
	{
		height = 1;
	}

	// set up current viewport
	glViewport(0, 0, width, height);
	mWidth = width;
	mHeight = height;

	// set default projection matrix
	float d = 1.0f / IvTan(mFOV / 180.0f * Types::s_PI * 0.5f);
	float recip = 1.0f / (mNear - mFar);
	IvMatrix44 perspective;

	perspective(0, 0) = d / ((GLfloat)width / (GLfloat)height);
	perspective(1, 1) = d;
	perspective(2, 2) = (mNear + mFar)*recip;
	perspective(2, 3) = 2 * mNear*mFar*recip;
	perspective(3, 2) = -1.0f;
	perspective(3, 3) = 0.0f;

	SetProjectionMatrix(perspective);

	IvMatrix44 ident;

	SetViewMatrix(ident);
	SetWorldMatrix(ident);

	// set default modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}   // End of IvRendererOGL::Resize()


Graphics::CRendererOGL::~CRendererOGL()
{
}

Int32 Graphics::CRendererOGL::InitializeGraphics(Int32 width, Int32 height, bool isFullScreen)
{
	GLenum err = GL_NO_ERROR;
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	// disable CULL FACE - nice to have in a future version :)
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

#ifdef _IS_EDITOR_
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	//ImGui_ImplGLUT_InstallFuncs();
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
#endif

	return 1;
}

Graphics::CRendererOGL::CRendererOGL() 
{}

//-------------------------------------------------------------------------------
// @ IvRendererOGL::SetProjectionMatrix()
//-------------------------------------------------------------------------------
// Sets the projection matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::CRendererOGL::SetProjectionMatrix(const IvMatrix44& matrix)
{
	IRenderer::SetProjectionMatrix(matrix);

	// set fixed function matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(mProjectionMat.GetFloatPtr());
}

//-------------------------------------------------------------------------------
// @ IvRendererOGL::SetWorldMatrix()
//-------------------------------------------------------------------------------
// Sets the world matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::CRendererOGL::SetWorldMatrix(const IvMatrix44& matrix)
{
	IRenderer::SetWorldMatrix(matrix);

	// set fixed function matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((mViewMat * mWorldMat).GetFloatPtr());
}

//-------------------------------------------------------------------------------
// @ IvRendererOGL::SetViewMatrix()
//-------------------------------------------------------------------------------
// Sets the camera matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::CRendererOGL::SetViewMatrix(const IvMatrix44& matrix)
{
	IRenderer::SetViewMatrix(matrix);

	// set fixed function matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((mViewMat * mWorldMat).GetFloatPtr());
}
