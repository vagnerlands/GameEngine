#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "CommonTypes.h"
#include "IvMatrix44.h"

using namespace Types;

namespace Graphics
{

	class IRenderer
	{
	public:
		// Create needs to be implemented in the derived Game class
		static bool Create();
		// Should delete objects in the inverse order of their creation
		static void Destroy();

		virtual void PrepareCamera3D() = 0;
		virtual void Resize(UInt32 width, UInt32 height) = 0;
		

		const IvMatrix44& GetWorldMatrix();
		virtual void SetWorldMatrix(const IvMatrix44& matrix);
		const IvMatrix44& GetViewMatrix();
		virtual void SetViewMatrix(const IvMatrix44& matrix);
		const IvMatrix44& GetProjectionMatrix();
		virtual void SetProjectionMatrix(const IvMatrix44& matrix);

		virtual Int32 InitializeGraphics(Int32 width, Int32 height, bool isFullScreen);

		static IRenderer*  mRenderer;          // global pointer

		virtual ~IRenderer();

	protected:
		// constructor/destructor
		IRenderer();

		bool            mIsActive;

		UInt32 mWidth;        // current width, height
		UInt32 mHeight;

		Float mFOV;
		Float mNear;
		Float mFar;

		//IvResourceManager*  mResourceManager;

		IvMatrix44        mWorldMat;
		IvMatrix44        mViewMat;
		IvMatrix44        mProjectionMat;

		IvMatrix44        mWVPMat;
		IvMatrix44        mNormalMat;

		IvVector4		  mDiffuseColor;
		IvVector4		  mLightAmbient;
		IvVector4		  mLightDiffuse;
		IvVector4		  mLightDirection;

	private:
		// copy operations
		IRenderer(const IRenderer& other);
		IRenderer& operator=(const IRenderer& other);
	};

}
#endif // _IRENDERER_H_