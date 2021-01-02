#ifndef _SceneItemAnimatedSimple_H_
#define _SceneItemAnimatedSimple_H_

#include "CommonTypes.h"
#include "SceneItemSimple.h"
#include "Ilumination.h"

using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Animated Scene Objects
	class SceneItemAnimatedSimple : public SceneItemSimple
	{
	public:
		SceneItemAnimatedSimple(const std::string& id, IDrawable* pDrawable) : SceneItemSimple(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemAnimatedSimple()
		{
		}

		//virtual void SetUpScene(cwc::glShader* pShader) const;
		virtual void SetUpAnimation(cwc::glShader* pShader) const override
		{
			pShader->setUniform1i("hasAnimations", 1);
		}
		//virtual void SetUpShadows(cwc::glShader* pShader) const;
		virtual void ShadowsPass() const override
		{
			Graphics::Ilumination::Instance().HasAnimations(1);
			Graphics::Ilumination::Instance().UpdateModel(m_model);
		}
	};
}


#endif // _SceneItemAnimatedSimple_H_