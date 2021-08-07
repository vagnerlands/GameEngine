#include "SceneItemFactory.h"

#include "SceneItem.h"
#include "SceneItemSimple.h"
#include "SceneItemSkyBox.h"
#include "SceneItemAnimatedAndShadowed.h"
#include "SceneItemAnimatedSimple.h"
#include "SceneItemShadowedSimple.h"
#include "SceneItemParticlesSystem.h"

shared_ptr<Graphics::SceneItem> Graphics::SceneItemFactory::Create(const std::string& id, shared_ptr<IDrawable> pDrawable, eSceneItemType type)
{
	switch (type)
	{
    case eSceneItemType_Regular: return make_shared<Graphics::SceneItem>(id, pDrawable);
	case eSceneItemType_Simple: return make_shared<Graphics::SceneItemSimple>(id, pDrawable);
	case eSceneItemType_SkyBox: return make_shared<Graphics::SceneItemSkyBox>(id, pDrawable);
	case eSceneItemType_Animated: return make_shared<Graphics::SceneItemAnimatedSimple>(id, pDrawable);
	case eSceneItemType_AnimatedAndShadowed: return make_shared<Graphics::SceneItemAnimatedAndShadowed>(id, pDrawable);
	case eSceneItemType_NonAnimatedAndShadowed: return make_shared<Graphics::SceneItemShadowedSimple>(id, pDrawable);
	case eSceneItemType_ParticlesSystem: return make_shared<Graphics::SceneItemParticlesSystem>(id, pDrawable, 100);
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
