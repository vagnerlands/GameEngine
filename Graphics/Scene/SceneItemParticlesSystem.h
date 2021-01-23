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
		SceneItemParticlesSystem(const std::string& id, IDrawable* pDrawable, Int32 NumberOfParticles) : SceneItemSimple(id, pDrawable), m_numberOfParticles(NumberOfParticles)
		{
			for (Int32 i = 0; i < NumberOfParticles; ++i)
			{
				Float speed = 1.f + (((rand() % 100)+1) / 100.f);
				Float size = 0.01f + ((((rand() % 10)) + 1) / 100.f);
				float height = 0.5f + (((rand() % 100) + 1) / 100.f);
				float spread = (((rand() % 50)-25) + 1) / 100.f;
				Int32 age = 100 + (rand() % 100) + 1;
				m_attributes.push_back(ParticleAttributes(speed, size, age, height, spread));
			}
			m_timetag = 1;
		}
		// virtual dtor
		~SceneItemParticlesSystem()
		{
		}

		Int32 GetNumberOfParticles() const
		{
			return m_numberOfParticles;
		}

		void SetUpParticleAttributes(cwc::glShader* pShader, Int32 index) const
		{
			pShader->setUniform1f("particleSize", m_attributes[index].Size);
			pShader->setUniform1f("particleSpeed", m_attributes[index].Speed);
			pShader->setUniform1f("particleHeight", m_attributes[index].MaxHeight);
			pShader->setUniform1f("particleAge", m_attributes[index].Age);
			pShader->setUniform1f("particleSpread", m_attributes[index].Spread);
			pShader->setUniform1f("time_0_X", m_timetag % m_attributes[index].Age);
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

			// Apply attributes known for this shader
			pShader->setUniform3f("cameraRight", CameraRight_worldspace.GetX(), CameraRight_worldspace.GetY(), CameraRight_worldspace.GetZ());
			pShader->setUniform3f("cameraUp", CameraUp_worldspace.GetX(), CameraUp_worldspace.GetY(), CameraUp_worldspace.GetZ());
			pShader->setUniform4f("particleSystemPosition", m_location.GetX(), m_location.GetY(), m_location.GetZ(), 0.f);
			char* dif = "diffuseMap";
			pShader->setTexture(dif, CTextureHolder::s_pInstance->getTextureById("flame.png"));

			m_timetag++;
			if (m_timetag > 100000) m_timetag = 1;

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
		struct ParticleAttributes
		{
			ParticleAttributes(Float speed, Float size, Int32 age, Float maxheight, Float spread) : Speed(speed), Size(size), Age(age), MaxHeight(maxheight), Spread(spread) {}
			Float Speed, Size;
			Float MaxHeight;
			float Spread;
			Int32 Age;
		};
		Int32 m_numberOfParticles;
		std::vector<ParticleAttributes> m_attributes;
		mutable Int32 m_timetag;
	};
}


#endif // _SceneItemParticlesSystem_H_