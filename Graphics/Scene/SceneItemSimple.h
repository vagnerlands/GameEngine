#ifndef _SceneItemSimple_H_
#define _SceneItemSimple_H_

#include "CommonTypes.h"
#include "SceneItem.h"
#include "Ilumination.h"

using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Shadowed Scene Objects
	class SceneItemSimple : public SceneItem
	{
	public:
		SceneItemSimple(const std::string& id, IDrawable* pDrawable) : SceneItem(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemSimple()
		{
		}
		virtual void SetUpScene(cwc::glShader* pShader) const
		{
			SceneItem::SetUpScene(pShader);
			pShader->setUniform2f("uvFactor", m_uvFactor.GetX(), m_uvFactor.GetY());
		}

		//virtual void SetUpAnimation(cwc::glShader* pShader) const override
		//{
		//	pShader->setUniform1i("hasAnimations", 0);
		//}
		//virtual void SetUpShadows(cwc::glShader* pShader) const override
		//{

		//}

		//virtual void SetUpShadows(cwc::glShader* pShader) const;
		//virtual void ShadowsPass() const override
		//{
		//	Graphics::Ilumination::Instance().HasAnimations(0);
		//	Graphics::Ilumination::Instance().UpdateModel(m_model);
		//}
	};
}


#endif // _SceneItemSimple_H_