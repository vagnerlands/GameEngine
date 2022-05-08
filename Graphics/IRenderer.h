#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "CommonTypes.h"
#include "IvMatrix44.h"
#include "Camera/CCamera.h"
#include "Camera/Frustum.h"

using namespace Types;

class IvAABB;

namespace Graphics
{

	class IRenderer
	{
	public:
		// Create needs to be implemented in the derived Game class
		static bool Create();
		// Should delete objects in the inverse order of their creation
		static void Destroy();
		// Procedures prior first rendering such as clearing previous buffer
		// defining what buffers, if it's double buffered and more...
		virtual void PrepareFrame() = 0;
		virtual void PrepareCamera2D() = 0;
		virtual void PrepareCamera3D() = 0;
        virtual void SwapBuffer() = 0;

		virtual void Resize(UInt32 width, UInt32 height) = 0;

		// getters
		virtual Float GetWidth();
		virtual Float GetHeight();
		virtual Float GetAspect();
		virtual Float GetFar();
		virtual Float GetNear();
        virtual Float GetBias();
        virtual Float GetShadowFactor();
		// setters
		virtual void SetDisplayWidth(UInt32 width);
		virtual void SetDisplayHeight(UInt32 height);
		virtual void SetNearPlane(Float near);
		virtual void SetFarPlane(Float far);
        virtual void SetBias(Float var);
        virtual void SetShadowFactor(Float var);
		virtual void SetFOV(Float fov);
		virtual void SetOrtho(Float l, Float r, Float t, Float b, Float n, Float f);
		virtual CCamera& GetCamera()
		{
			return mCamera;
		}
		

		const IvMatrix44& GetWorldMatrix();
		virtual void SetWorldMatrix(const IvMatrix44& matrix);
		IvMatrix44& GetViewMatrix();
		virtual void SetViewMatrix(const IvMatrix44& matrix);
		IvMatrix44& GetProjectionMatrix();
		virtual void SetProjectionMatrix(const IvMatrix44& matrix);
        bool IsClip(const IvVector3& point);
        bool IsClip(const IvAABB& aabb);

		virtual Int32 InitializeGraphics(Int32 width, Int32 height, bool isFullScreen);

		virtual bool IsOperational() = 0;

		static IRenderer*  mRenderer;          // global pointer

		virtual ~IRenderer();

	protected:
		// constructor/destructor
		IRenderer();

		bool   mIsActive;

		UInt32 mWidth;        // current width, height
		UInt32 mHeight;

		// Parameters for Perspective rendering
		Float mFOV;
		Float mNear;
		Float mFar;
        Float mBias;
        Float mShadowFactor;

		// Parameters for Orthographic rendering
		Float mRight;
		Float mLeft;
		Float mTop;
		Float mBottom;

		// Matrices 
		IvMatrix44        mWorldMat;
		IvMatrix44        mViewMat;
		IvMatrix44        mProjectionMat;

		IvMatrix44        mWVPMat;
		IvMatrix44        mNormalMat;

		IvVector4		  mDiffuseColor;
		IvVector4		  mLightAmbient;
		IvVector4		  mLightDiffuse;
		IvVector4		  mLightDirection;

		CCamera           mCamera;
        Frustum           mFrustum;

	private:
		// copy operations
		IRenderer(const IRenderer& other);
		IRenderer& operator=(const IRenderer& other);
	};


	inline Float 
		IRenderer::GetWidth()
	{
		return static_cast<Float>(mWidth);
	}

	inline Float 
		IRenderer::GetHeight()
	{
		return static_cast<Float>(mHeight);
	}

	inline Float 
		IRenderer::GetAspect()
	{
		return static_cast<Float>(mWidth) / static_cast<Float>(mHeight);
	}

	inline Float
		IRenderer::GetFar()
	{
		return mFar;
	}

	inline Float
		IRenderer::GetNear()
	{
		return mNear;
	}

    inline Float
        IRenderer::GetBias()
    {
        return mBias;
    }

    inline void
        IRenderer::SetBias(Float var)
    {
        mBias = var;
    }

    inline Float
        IRenderer::GetShadowFactor()
    {
        return mShadowFactor;
    }

    inline void
        IRenderer::SetShadowFactor(Float var)
    {
        mShadowFactor = var;
    }

	//-------------------------------------------------------------------------------
	// @ IRendererOGL::SetDisplayWidth()
	//-------------------------------------------------------------------------------
	// Set up GL the view width - this won't affect the window size, just the 
	// viewport
	//-------------------------------------------------------------------------------
	inline void 
	IRenderer::SetDisplayWidth(UInt32 width)
	{
		mWidth = width;
	}

	//-------------------------------------------------------------------------------
	// @ IRendererOGL::SetDisplayHeight()
	//-------------------------------------------------------------------------------
	// Set up GL the view height - this won't affect the window size, just the 
	// viewport
	//-------------------------------------------------------------------------------
	inline void 
	IRenderer::SetDisplayHeight(UInt32 height)
	{
		mHeight = height;
	}

	//-------------------------------------------------------------------------------
	// @ IRendererOGL::SetNearPlane()
	//-------------------------------------------------------------------------------
	// Set up GL near plane value (how close is the camera clip)
	// We're using centimeters here.
	//-------------------------------------------------------------------------------
	inline void 
	IRenderer::SetNearPlane(Float near)
	{
		//mNear = near;
	}

	//-------------------------------------------------------------------------------
	// @ IRendererOGL::SetFarPlane()
	//-------------------------------------------------------------------------------
	// Set up GL far plane value (how far the camera can see)
	// We're using centimeters here.
	//-------------------------------------------------------------------------------
	inline void 
		IRenderer::SetFarPlane(Float far)
	{
		//mFar = far;
	}

	//-------------------------------------------------------------------------------
	// @ IRendererOGL::SetFOV()
	//-------------------------------------------------------------------------------
	// Set up GL field of view in degrees
	//-------------------------------------------------------------------------------
	inline void 
	IRenderer::SetFOV(Float fov)
	{
		mFOV = fov;
	}

	inline void 
	IRenderer::SetOrtho(Float l, Float r, Float t, Float b, Float n, Float f)
	{
		mLeft	= l;
		mRight	= r;
		mTop	= t;
		mBottom = b;
		mNear	= n;
		mFar	= f;
	}

}
#endif // _IRENDERER_H_