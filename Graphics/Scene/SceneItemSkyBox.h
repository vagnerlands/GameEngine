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
	};
}


#endif // _SceneItemAnimatedAndShadowed_H_