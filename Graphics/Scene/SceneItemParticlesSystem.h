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
	struct ParticlesAttributes
	{
		IvVector4 ParticlePosition;
		Float ParticleSize;
	};

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
			IvMatrix44& viewMatrix		= Graphics::IRenderer::mRenderer->GetViewMatrix();
			IvMatrix44& projMatrix		= Graphics::IRenderer::mRenderer->GetProjectionMatrix();
			pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
			pShader->setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());
			IvVector4 cols[4];
			viewMatrix.GetColumns(cols[0], cols[1], cols[2],cols[3]);
			IvVector3 CameraRight_worldspace(cols[0].GetX(), cols[0].GetY(), cols[0].GetZ());
			IvVector3 CameraUp_worldspace(cols[1].GetX(), cols[1].GetY(), cols[1].GetZ());

			static Float time_0_X = 0.f;
			time_0_X += 0.16666f;

			// Apply attributes known for this shader
			//pShader->setUniform1f("time_0_X", time_0_X);
			pShader->setUniform3f("cameraRight", CameraRight_worldspace.GetX(), CameraRight_worldspace.GetY(), CameraRight_worldspace.GetZ());
			pShader->setUniform3f("cameraUp", CameraUp_worldspace.GetX(), CameraUp_worldspace.GetY(), CameraUp_worldspace.GetZ());
			pShader->setUniform1f("particleSize", 1.0f);
			pShader->setUniform4f("particleSystemPosition", 0.f, 1.5f, 3.f, 0.f);
			char* dif = "diffuseMap";
			pShader->setTexture(dif, CTextureHolder::s_pInstance->getTextureById("blob.png"));

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

	private:
		std::vector<ParticlesAttributes>* m_pParticlesAttributes;
	};
}


#endif // _SceneItemParticlesSystem_H_