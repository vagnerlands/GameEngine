#include "RenderScene.h"
#include "SceneItemFactory.h"

Graphics::RenderScene & Graphics::RenderScene::Instance()
{
	static Graphics::RenderScene instance;
	return instance;
}

Graphics::SceneItem& Graphics::RenderScene::Add(const std::string& id, shared_ptr<IDrawable> pDrawable, eSceneItemType type)
{	
    auto scene = SceneItemFactory::Instance().Create(id, pDrawable, type);
	// add to the current rendering database
	m_items.push_back(scene);
    return *scene;
}

void Graphics::RenderScene::Change(const std::string& id, shared_ptr<IDrawable> pDrawable)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->ReplaceDrawable(pDrawable);
	}

}

void Graphics::RenderScene::Translate(const std::string & id, const IvVector3 & newLocation)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetLocation(newLocation);
	}
}

void Graphics::RenderScene::Rotate(const std::string & id, const IvQuat & newLocation)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetRotation(newLocation);
	}
}

void Graphics::RenderScene::Scale(const std::string & id, const IvVector3 & newScale)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetScale(newScale);
	}
}

void Graphics::RenderScene::SetTextureUV(const std::string& id, const IvVector2& uvFactor)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetTextureUV(uvFactor);
	}
}

void Graphics::RenderScene::CastShadow(const std::string & id, bool hasShadow)
{
    shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetCastShadows(hasShadow);
	}
}

void Graphics::RenderScene::Remove(const std::string & id)
{
    shared_ptr <Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
        pObj->Release(); // drop a reference to the underlying drawable
		m_items.remove(pObj);
	}
}

void Graphics::RenderScene::ApplyQuery(const std::string& id, SceneQuery& query)
{
	shared_ptr<Graphics::SceneItem> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->Query(query);
	}
}

void Graphics::RenderScene::Render(float dt, bool isRenderingShadows) const
{
	for (auto& it = m_items.begin(); it != m_items.end(); it++)
	{
		shared_ptr<Graphics::SceneItem> pObj = *it;
		pObj->Render(dt, isRenderingShadows);
	}
}

shared_ptr<Graphics::SceneItem> Graphics::RenderScene::find(const std::string & id)
{
	for (auto& it = m_items.begin(); it != m_items.end(); it++)
	{
		if (*(*it) == id)
		{
			return *it;
		}
	}

	return nullptr;
}

void Graphics::RenderScene::Destroy()
{
    m_items = DatabaseSceneItemsType();
}

void Graphics::RenderScene::DisplayBoundingBox(const std::string& id, bool display)
{
    auto sceneItem = find(id);
    if (sceneItem != nullptr)
    {
        sceneItem->DisplayBoundingBox(display);
    }
}
