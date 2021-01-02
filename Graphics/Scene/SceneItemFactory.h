#ifndef _AnimationStyleFactory_H_
#define _AnimationStyleFactory_H_

#include "CommonTypes.h"

using namespace Types;

namespace Graphics
{
	class SceneItem;
	class IDrawable;
	// Gives extra behavior for Animated Scene Objects
	class SceneItemFactory
	{
	public:
		// virtual dtor
		~SceneItemFactory()
		{
		}

		SceneItem* Create(const std::string& id, IDrawable* pDrawable, eSceneItemType type);

		static SceneItemFactory& Instance();
	private:
		SceneItemFactory()
		{

		}
		
	};
}


#endif // _AnimationStyleFactory_H_