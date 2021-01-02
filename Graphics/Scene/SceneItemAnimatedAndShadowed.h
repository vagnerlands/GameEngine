#ifndef _SceneItemAnimatedAndShadowed_H_
#define _SceneItemAnimatedAndShadowed_H_

#include "CommonTypes.h"
#include "SceneItemSimple.h"
#include "Ilumination.h"

using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Shadowed Scene Objects
	class SceneItemAnimatedAndShadowed : public SceneItemSimple
	{
	public:
		SceneItemAnimatedAndShadowed(const std::string& id, IDrawable* pDrawable) : SceneItemSimple(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemAnimatedAndShadowed()
		{
		}
		//virtual void SetUpScene(cwc::glShader* pShader) const;
		virtual void SetUpAnimation(cwc::glShader* pShader) const override
		{
			pShader->setUniform1i("hasAnimations", 1);
		}
		virtual void SetUpShadows(cwc::glShader* pShader) const override
		{

		}

		//virtual void SetUpShadows(cwc::glShader* pShader) const;
		virtual void ShadowsPass() const override
		{
			Graphics::Ilumination::Instance().HasAnimations(1);
			Graphics::Ilumination::Instance().UpdateModel(m_model);
		}
	};
}


#endif // _SceneItemAnimatedAndShadowed_H_