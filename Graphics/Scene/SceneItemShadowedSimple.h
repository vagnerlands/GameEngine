#ifndef _SceneItemShadowedSimple_H_
#define _SceneItemShadowedSimple_H_

#include "CommonTypes.h"
#include "SceneItemSimple.h"
#include "Ilumination.h"

using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Shadowed Scene Objects
	class SceneItemShadowedSimple : public SceneItemSimple
	{
	public:
		SceneItemShadowedSimple(const std::string& id, shared_ptr<Graphics::IDrawable> pDrawable) : SceneItemSimple(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemShadowedSimple()
		{
		}

		virtual void SetUpAnimation(cwc::glShader* pShader) const override
		{
			pShader->setUniform1i("hasAnimations", 0);
		}
		//virtual void SetUpShadows(cwc::glShader* pShader) const;
		virtual void ShadowsPass() const override
		{
			Graphics::Ilumination::Instance().HasAnimations(0);
			Graphics::Ilumination::Instance().UpdateModel(m_model);
		}
	};
}


#endif // _SceneItemShadowedSimple_H_