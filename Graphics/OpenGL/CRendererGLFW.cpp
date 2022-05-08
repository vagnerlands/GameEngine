#include "CRendererGLFW.h"
#include "TextRendererOGL.h"
#include "IvMath.h"
#include "IvMatrix33.h"
#include <iostream>

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")

#include "imgui.h"
#include "imgui/res/bindings/imgui_impl_glfw.h"
#include "imgui/res/bindings/imgui_impl_opengl3.h"


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


bool Graphics::CRendererGLFW::Create()
{
	if (mRenderer == nullptr)
	{
		mRenderer = new CRendererGLFW();
	}
	return (mRenderer != nullptr);
}

void Graphics::CRendererGLFW::Destroy()
{
	if (mRenderer != nullptr)
	{
		delete mRenderer;
	}

	mRenderer = 0;
}

void Graphics::CRendererGLFW::PrepareFrame()
{
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();


	int display_w, display_h;
	glfwGetFramebufferSize(mWindow, &display_w, &display_h);

	if ((mWidth != display_w) || (mHeight != display_h))
		Resize(display_w, display_h);

	// check for errors
	Int32 glErr = 0;
	while (glErr=glGetError() != 0)
		DEBUG_OUT("Failed to apply shader ");

}
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/glm.hpp>
void Graphics::CRendererGLFW::PrepareCamera2D()
{
    glClear(GL_DEPTH_BUFFER_BIT); // avoid 2d objects to be "cut" by 3d objects in the scene

	IvMatrix44 ortho;

    const Float cNear   = -1.0;
    const Float cFar    =  1.0;

	Float recipX = (mWidth);
	Float recipY = (mHeight);
	Float recipZ = (cFar - cNear);


	ortho(0, 0) = 2.0f/recipX;
    ortho(1, 1) = 2.0f/recipY;
    ortho(2, 2) = 1.f; //2.0f/recipZ;

	ortho(0, 3) = -(mWidth/recipX);
	ortho(1, 3) = -(mHeight/recipY);
	//ortho(2, 3) =  ((cFar + cNear)/recipZ);

	int v = glGetError();

    SetProjectionMatrix(ortho);    
	v = glGetError();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	UInt32 counter = 0;
	Float f = 0.f;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::Begin("World Configuration");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("Display element:");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Warrior_1", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Warrior_2", &show_another_window);
		ImGui::Checkbox("Warrior_3", &show_another_window);
		ImGui::Checkbox("Mage_1", &show_another_window);
		ImGui::Checkbox("Mage_2", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Graphics::CRendererGLFW::PrepareCamera3D()
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
	//while ((glErr = glGetError()) != GL_NO_ERROR)
	//printf("Failed to set CRendererGLFW::PrepareCamera3D SetProjectionMatrix %d\n", glErr);
	IvMatrix44 ident;
	SetViewMatrix(ident);
	SetWorldMatrix(ident);
	//while ((glErr = glGetError()) != GL_NO_ERROR)
	//	printf("Failed to set CRendererGLFW::PrepareCamera3D SetWorldMatrix %d\n", glErr);
	
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
	//while ((glErr = glGetError()) != GL_NO_ERROR)
	//	printf("Failed to set CRendererGLFW::PrepareCamera3D SetViewMatrix %d\n", glErr);
    mFrustum.CalculateFrustum();
	//while ((glErr = glGetError()) != GL_NO_ERROR)
	//	printf("Failed to set CRendererGLFW::PrepareCamera3D %d\n", glErr);
}

void Graphics::CRendererGLFW::SwapBuffer()
{
	glfwSwapBuffers(mWindow);
}

//-------------------------------------------------------------------------------
// @ CRendererOGL::Resize()
//-------------------------------------------------------------------------------
// Set up GL parameters for window size
//-------------------------------------------------------------------------------
void
	Graphics::CRendererGLFW::Resize(UInt32 width, UInt32 height)
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


Graphics::CRendererGLFW::~CRendererGLFW()
{
}

Int32 Graphics::CRendererGLFW::InitializeGraphics(Int32 width, Int32 height, bool isFullScreen)
{

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return -1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
	GLenum err = GL_NO_ERROR;
	// Create window with graphics context
	mWindow = glfwCreateWindow(1280, 720, "Game Engine Editor (OpenGL - GLFW)", NULL, NULL);
	if (mWindow == NULL)
		return -2;

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1); // Enable vsync
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
	// disable CULL FACE - nice to have in a future version :)
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
	TextRendererOGL::Create();
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
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
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "GL Error at " << __FILE__ << ":" << __LINE__ << ", Reason: " << std::to_string(err) << std::endl;
	return 1;
}

bool Graphics::CRendererGLFW::IsOperational()
{
	glfwMakeContextCurrent(mWindow);
	return !glfwWindowShouldClose(mWindow);
}

Graphics::CRendererGLFW::CRendererGLFW()
{}

//-------------------------------------------------------------------------------
// @ IvRendererOGL::SetProjectionMatrix()
//-------------------------------------------------------------------------------
// Sets the projection matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::CRendererGLFW::SetProjectionMatrix(const IvMatrix44& matrix)
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
void Graphics::CRendererGLFW::SetWorldMatrix(const IvMatrix44& matrix)
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
void Graphics::CRendererGLFW::SetViewMatrix(const IvMatrix44& matrix)
{
	IRenderer::SetViewMatrix(matrix);

	// set fixed function matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((mViewMat * mWorldMat).GetFloatPtr());
}
