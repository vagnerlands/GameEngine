#include "SceneItemFactory.h"

#include "SceneItem.h"
#include "SceneItemAnimatedAndShadowed.h"
#include "SceneItemAnimatedSimple.h"
#include "SceneItemShadowedSimple.h"

Graphics::SceneItem* Graphics::SceneItemFactory::Create(const std::string& id, IDrawable* pDrawable, eSceneItemType type)
{
	switch (type)
	{
	case eSceneItemType_Regular: return new SceneItem(id, pDrawable);
	case eSceneItemType_Animated: return new SceneItemAnimatedSimple(id, pDrawable);
	case eSceneItemType_AnimatedAndShadowed: return new SceneItemAnimatedAndShadowed(id, pDrawable);
	case eSceneItemType_NonAnimatedAndShadowed: return new SceneItemShadowedSimple(id, pDrawable);
	default:
		break;
	}
}

Graphics::SceneItemFactory& Graphics::SceneItemFactory::Instance()
{
	static SceneItemFactory instance;
	return instance;
}
