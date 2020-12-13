#include "RenderScene.h"

Graphics::RenderScene & Graphics::RenderScene::Instance()
{
	static Graphics::RenderScene instance;
	return instance;
}

void Graphics::RenderScene::Add(const std::string& id, IDrawable* pDrawable)
{	
	// add to the current rendering database
	m_items.push_back(new SceneItem(id, pDrawable));
}

void Graphics::RenderScene::Translate(const std::string & id, const IvVector3 & newLocation)
{
	Graphics::SceneItem* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetLocation(newLocation);
	}
}

void Graphics::RenderScene::Rotate(const std::string & id, const IvQuat & newLocation)
{
	Graphics::SceneItem* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetRotation(newLocation);
	}
}

void Graphics::RenderScene::Scale(const std::string & id, const IvVector3 & newScale)
{
	Graphics::SceneItem* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetScale(newScale);
	}
}

void Graphics::RenderScene::CastShadow(const std::string & id, bool hasShadow)
{
	Graphics::SceneItem* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetCastShadows(hasShadow);
	}
}

void Graphics::RenderScene::Remove(const std::string & id)
{
	Graphics::SceneItem* pObj = find(id);
	if (pObj != nullptr)
	{
		m_items.remove(pObj);
		delete pObj;
	}
}

//void Graphics::RenderScene::Update(float dt) const
//{
//	for (const auto& it = m_items.begin(); it != m_items.end(); it++)
//	{
//		Graphics::SceneItem* pObj = *it;
//		pObj->Update(dt);
//	}
//}

void Graphics::RenderScene::Render(float dt, bool isRenderingShadows) const
{
	for (auto& it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::SceneItem* pObj = *it;
		pObj->Render(dt, isRenderingShadows);
	}
}

Graphics::SceneItem* Graphics::RenderScene::find(const std::string & id)
{
	Graphics::SceneItem* retVal = nullptr;

	for (auto& it = m_items.begin(); it != m_items.end(); it++)
	{
		auto& pObj = *it;
		if (*pObj == id)
		{
			retVal = *it;
			break;
		}
	}

	return retVal;
}
