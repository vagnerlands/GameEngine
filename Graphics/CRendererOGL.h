#ifndef _CRENDEREROGL_H_
#define _CRENDEREROGL_H_

#include "CommonTypes.h"
#include "IRenderer.h"

using namespace Types;

namespace Graphics
{

	class CRendererOGL : public IRenderer
	{
	public:
		static bool Create();

		static void Destroy();

		virtual void PrepareFrame() override;

		virtual void PrepareCamera2D() override;

		virtual void PrepareCamera3D() override;

		virtual void Resize(UInt32 width, UInt32 height) override;

		virtual void SetProjectionMatrix(const IvMatrix44& matrix) override;
		virtual void SetViewMatrix(const IvMatrix44& matrix) override;
		virtual void SetWorldMatrix(const IvMatrix44& matrix) override;

		// we want inherited classes to be able to delete this object
		virtual ~CRendererOGL();

		Int32 InitializeGraphics(Int32 width, Int32 height, bool isFullScreen) override;

	protected:
		// only possible to create this object through "Create()" method
		CRendererOGL();

	private:
		// copy operations
		CRendererOGL(const CRendererOGL& other);
		CRendererOGL& operator=(const CRendererOGL& other);
	};

}
#endif // _CRENDEREROGL_H_