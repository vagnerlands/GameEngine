#ifndef _CPARTICLE_H_
#define _CPARTICLE_H_

#include "CommonTypes.h"
#include "IMutex.h"
#include "IDrawable.h"
#include "glsl.h"

#include "IvMatrix44.h"

using namespace Types;

namespace Graphics
{
	class CParticle : public IDrawable
	{
	public:
		CParticle(string id);
		virtual ~CParticle();
		void Create();
		// periodic call to update
		virtual void Update(float dt) override;
		//bool SetShader(const string& shaderName);
		virtual void Draw(const SceneItem& si, float dt, bool isRenderingShadows) override;
        virtual const IvAABB* GetBoundaryBox() const override;

		void SetLoopCount(Int32 loopCount = -1);
		void SetMaxNumberOfParticles(Int32 maxNumberOfParticles);
		void SetVariants(Int32 spread, Int32 speed, Int32 height, Int32 age, Int32 size);
		void SetParticlesPosition(IvVector3& position);
		void SetTexture(const string& textureId);

	protected:

		cwc::glShader* generateShader(const string& shaderName);

		// copy operations
		CParticle(const CParticle& other) = delete;
		CParticle& operator=(const CParticle& other) = delete;

		UInt32 VAO, VBO = 0;

		std::string				m_particleName;

		cwc::glShader*			m_pShader;



	};


}
#endif // _CParticle_H_