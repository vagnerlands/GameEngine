#pragma once

#include "CommonTypes.h"
#include "IRenderer.h"

using namespace Types;

struct GLFWwindow;

namespace Graphics
{

	class CRendererGLFW : public IRenderer
	{
	public:
		static bool Create();

		static void Destroy();

		virtual void PrepareFrame() override;

		virtual void PrepareCamera2D() override;

		virtual void PrepareCamera3D() override;

        virtual void SwapBuffer() override;

		virtual void Resize(UInt32 width, UInt32 height) override;

		virtual void SetProjectionMatrix(const IvMatrix44& matrix) override;
		virtual void SetViewMatrix(const IvMatrix44& matrix) override;
		virtual void SetWorldMatrix(const IvMatrix44& matrix) override;

		// we want inherited classes to be able to delete this object
		virtual ~CRendererGLFW();

		Int32 InitializeGraphics(Int32 width, Int32 height, bool isFullScreen) override;

		bool IsOperational() override;

	protected:
		// only possible to create this object through "Create()" method
		CRendererGLFW();

	private:
		// copy operations
		CRendererGLFW(const CRendererGLFW& other);
		CRendererGLFW& operator=(const CRendererGLFW& other);

		// global
		GLFWwindow* mWindow = nullptr;
	};

}