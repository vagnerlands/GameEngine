#include "SceneItemFactory.h"

#include "SceneItem.h"
#include "SceneItemSimple.h"
#include "SceneItemSkyBox.h"
#include "SceneItemAnimatedAndShadowed.h"
#include "SceneItemAnimatedSimple.h"
#include "SceneItemShadowedSimple.h"
#include "SceneItemParticlesSystem.h"

Graphics::SceneItem* Graphics::SceneItemFactory::Create(const std::string& id, shared_ptr<IDrawable> pDrawable, eSceneItemType type)
{
	switch (type)
	{
	case eSceneItemType_Regular: return new SceneItem(id, pDrawable);
	case eSceneItemType_Simple: return new SceneItemSimple(id, pDrawable);
	case eSceneItemType_SkyBox: return new SceneItemSkyBox(id, pDrawable);
	case eSceneItemType_Animated: return new SceneItemAnimatedSimple(id, pDrawable);
	case eSceneItemType_AnimatedAndShadowed: return new SceneItemAnimatedAndShadowed(id, pDrawable);
	case eSceneItemType_NonAnimatedAndShadowed: return new SceneItemShadowedSimple(id, pDrawable);
	case eSceneItemType_ParticlesSystem: return new SceneItemParticlesSystem(id, pDrawable, 100);
	default:
		break;
	}
    return nullptr;
}

Graphics::SceneItemFactory& Graphics::SceneItemFactory::Instance()
{
	static SceneItemFactory instance;
	return instance;
}
