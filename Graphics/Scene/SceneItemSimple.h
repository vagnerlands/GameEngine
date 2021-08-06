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
		SceneItemSimple(const std::string& id, shared_ptr<Graphics::IDrawable> pDrawable) : SceneItem(id, pDrawable)
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
	};
}


#endif // _SceneItemSimple_H_