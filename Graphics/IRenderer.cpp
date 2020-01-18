#include "IRenderer.h"
#include "IvMatrix33.h"

// singleton object
Graphics::IRenderer* Graphics::IRenderer::mRenderer = nullptr;

//-------------------------------------------------------------------------------
// @ IvRenderer::IvRenderer()
//-------------------------------------------------------------------------------
// Default constructor
//-------------------------------------------------------------------------------
Graphics::IRenderer::IRenderer() :
	mIsActive(true),
	mWidth(640),
	mHeight(480),
	mFOV(45.0F),
	mNear(0.1F),
	mFar(1000.F)
{

	mDiffuseColor.Set(1.0f, 1.0f, 1.0f, 1.0f);

	mLightAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
	mLightDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
	mLightDirection = IvVector4::xAxis;

}   // End of IvRenderer::IvRenderer()




	//-------------------------------------------------------------------------------
	// @ IvRenderer::~IvRenderer()
	//-------------------------------------------------------------------------------
	// Destructor
	//-------------------------------------------------------------------------------
Graphics::IRenderer::~IRenderer()
{

}   // End of Destructor

bool Graphics::IRenderer::Create()
{
	// must create through the concrete implementation...
	return false;
}

void Graphics::IRenderer::Destroy()
{
	if (mRenderer != nullptr)
	{
		delete mRenderer;
	}

	mRenderer = 0;
}

Int32 Graphics::IRenderer::InitializeGraphics(Int32 width, Int32 height, bool isFullScreen)
{
	// this should be inherited
	return Int32();
}

//-------------------------------------------------------------------------------
// @ IvRenderer::GetWorldMatrix()
//-------------------------------------------------------------------------------
// Gets the world matrix for the renderer
//-------------------------------------------------------------------------------
const IvMatrix44& Graphics::IRenderer::GetWorldMatrix()
{
	return mWorldMat;
}

//-------------------------------------------------------------------------------
// @ IvRenderer::SetWorldMatrix()
//-------------------------------------------------------------------------------
// Sets the world matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::IRenderer::SetWorldMatrix(const IvMatrix44& matrix)
{
	mWorldMat = matrix;
	mWVPMat = mProjectionMat*mViewMat*mWorldMat;
	IvMatrix33 worldMat3x3;
	// inverse rows by cols - the GPU expects the information this way to be digested
	IvVector3 col0(mWorldMat(0, 0), mWorldMat(1, 0), mWorldMat(2, 0));
	IvVector3 col1(mWorldMat(0, 1), mWorldMat(1, 1), mWorldMat(2, 1));
	IvVector3 col2(mWorldMat(0, 2), mWorldMat(1, 2), mWorldMat(2, 2));
	worldMat3x3.SetColumns(col0, col1, col2);
	// find the normal matrix
	mNormalMat.Rotation(Transpose(Inverse(worldMat3x3)));
}

//-------------------------------------------------------------------------------
// @ IvRenderer::GetViewMatrix()
//-------------------------------------------------------------------------------
// Gets the camera matrix for the renderer
//-------------------------------------------------------------------------------
const IvMatrix44& Graphics::IRenderer::GetViewMatrix()
{
	return mViewMat;
}

//-------------------------------------------------------------------------------
// @ IvRenderer::SetViewMatrix()
//-------------------------------------------------------------------------------
// Sets the camera matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::IRenderer::SetViewMatrix(const IvMatrix44& matrix)
{
	mViewMat = matrix;
	mWVPMat = mProjectionMat*mViewMat*mWorldMat;
}

//-------------------------------------------------------------------------------
// @ IvRenderer::GetProjectionMatrix()
//-------------------------------------------------------------------------------
// Gets the projection matrix for the renderer
//-------------------------------------------------------------------------------
const IvMatrix44& Graphics::IRenderer::GetProjectionMatrix()
{
	return mProjectionMat;
}

//-------------------------------------------------------------------------------
// @ IvRenderer::SetProjectionMatrix()
//-------------------------------------------------------------------------------
// Sets the projection matrix for the renderer
//-------------------------------------------------------------------------------
void Graphics::IRenderer::SetProjectionMatrix(const IvMatrix44& matrix)
{
	mProjectionMat = matrix;
	mWVPMat = mProjectionMat*mViewMat*mWorldMat;
}

