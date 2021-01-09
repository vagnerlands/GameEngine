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

	protected:

		cwc::glShader* generateShader(const string& shaderName);

		// copy operations
		CParticle(const CParticle& other) = delete;
		CParticle& operator=(const CParticle& other) = delete;

		UInt32 VAO, VBO, EBO = 0;

		Int32                   m_indicesCount;
 

		std::string				m_particleName;

		cwc::glShader*			m_pShader;

	};


}
#endif // _CParticle_H_