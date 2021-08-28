#ifndef _SceneItemSkyBox_H_
#define _SceneItemSkyBox_H_

#include "CommonTypes.h"
#include "SceneItem.h"
#include "Ilumination.h"

using namespace Types;

namespace Graphics
{
	// Gives extra behavior for Shadowed Scene Objects
	class SceneItemSkyBox : public SceneItem
	{
	public:
		SceneItemSkyBox(const std::string& id, shared_ptr<Graphics::IDrawable> pDrawable) : SceneItem(id, pDrawable)
		{}
		// virtual dtor
		~SceneItemSkyBox()
		{
		}
        virtual void SetUpScene(cwc::glShader* pShader) const
        {
            SceneItem::SetUpScene(pShader);
            const IvVector3& ac = Graphics::Ilumination::Instance().GetAmbientLightColor();
            pShader->setUniform3f("ambient_color", ac.GetX(), ac.GetY(), ac.GetZ());
        }
	};
}


#endif // _SceneItemAnimatedAndShadowed_H_