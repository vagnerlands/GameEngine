#ifndef _SceneItemParticlesSystem_H_
#define _SceneItemParticlesSystem_H_

#include "CommonTypes.h"
#include "SceneItemSimple.h"
//#include "Ilumination.h"
#include "IRenderer.h"
#include "CTextureHolder.h"


using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Shadowed Scene Objects
	class SceneItemParticlesSystem : public SceneItemSimple
	{
	public:
		SceneItemParticlesSystem(const std::string& id, IDrawable* pDrawable) : SceneItemSimple(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemParticlesSystem()
		{
		}

		virtual void SetUpScene(cwc::glShader* pShader) const
		{
			// get shader program id 
			GLuint programId = pShader->GetProgramObject();
			// enable it
			glUseProgram(programId);
			// check for errors
			Int32 glErr = glGetError();
			if (glErr != 0)
				DEBUG_OUT("Failed to apply shader ");

			// Apply attributes known for this shader
			//IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
			IvMatrix44& viewMatrix			= Graphics::IRenderer::mRenderer->GetViewMatrix();
			IvMatrix44  inverseViewMatrix	= viewMatrix;
			inverseViewMatrix.AffineInverse();
			static Float time_0_X = 0.f;
			time_0_X += 0.16666f;

			// Apply attributes known for this shader
			IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
			pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
			pShader->setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());

			pShader->setUniformMatrix4fv("view_inverse_matrix", 1, false, (GLfloat*)inverseViewMatrix.GetFloatPtr());
			pShader->setUniform1f("time_0_X", time_0_X);
			pShader->setUniform1f("particleSystemHeight", 1.f);
			pShader->setUniform1f("particleSpeed", 0.005f);
			pShader->setUniform1f("particleSpread", 5.f);
			pShader->setUniform1f("particleSystemShape", 1.f);
			pShader->setUniform1f("particleShape", 0.37f);
			pShader->setUniform1f("particleSize", 0.5f);
			pShader->setUniform4f("particleSystemPosition", 0.f, 1.f, 3.f, 0.f);
			char* dif = "diffuseMap";
			pShader->setTexture(dif, CTextureHolder::s_pInstance->getTextureById("Flame.bmp"));

		}

		virtual void SetUpAnimation(cwc::glShader* pShader) const override
		{
			//pShader->setUniform1i("hasAnimations", 0);
		}
		//virtual void SetUpShadows(cwc::glShader* pShader) const;
		virtual void ShadowsPass() const override
		{
			//Graphics::Ilumination::Instance().HasAnimations(0);
			//Graphics::Ilumination::Instance().UpdateModel(m_model);
		}
	};
}


#endif // _SceneItemParticlesSystem_H_