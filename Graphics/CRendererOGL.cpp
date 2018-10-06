#include "CRendererOGL.h"
#include "IvMath.h"
#include "GL/glut.h"

bool Graphics::CRendererOGL::Create()
{
	if (mRenderer == nullptr)
	{
		mRenderer = new CRendererOGL();
	}
	return (mRenderer != nullptr);
}

void Graphics::CRendererOGL::Destroy()
{
	if (mRenderer != nullptr)
	{
		delete mRenderer;
	}

	mRenderer = 0;
}

void Graphics::CRendererOGL::PrepareCamera3D()
{
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


	return 1;
}

Graphics::CRendererOGL::CRendererOGL() 
{
	
}

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
