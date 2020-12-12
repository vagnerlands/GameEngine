#ifndef _ShadowsOGL_H_
#define _ShadowsOGL_H_

#include "CommonTypes.h"
#include "Shadows.h"
#include "Shaders/glsl.h"

using namespace Types;

namespace Graphics
{
	// Database of light sources
	class ShadowsOGL : public Shadows
	{
	public:
		ShadowsOGL() : m_depthCubemap(0), m_depthMapFBO(0), m_pShader(nullptr)
		{

		}
		// virtual destructor
		virtual ~ShadowsOGL()
		{

		}

		bool Initialize() override;

		void Start(const IvVector3& lightPos) override;

		void Stop() override;

		UInt32 GetDepthMapId() const override;

        void SetModel(IvMatrix44& model) override;

		void HasAnimations(bool value) override;

		void UpdateBoneTransformations(Float* boneTransformMat44, UInt32 boneIndex) override;

        void BindShadowTexture() override;


	protected:

	private:
		//! Depth cubemap allocated using opengl
		UInt32 m_depthCubemap;
		//! Shadow Cubemap FBO Id
		UInt32 m_depthMapFBO;
		//! shader for shadows
		cwc::glShader* m_pShader;
		static const UInt32 SHADOW_WIDTH = 1024;
		static const UInt32 SHADOW_HEIGHT = 1024;
		UInt32 m_bone_location[MAX_BONES];

		// copy operations
		ShadowsOGL(const ShadowsOGL& other);
		ShadowsOGL& operator=(const ShadowsOGL& other);
	};


}
#endif // _ShadowsOGL_H_