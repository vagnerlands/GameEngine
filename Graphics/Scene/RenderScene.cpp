#include "RenderScene.h"

Graphics::RenderScene & Graphics::RenderScene::Instance()
{
	static Graphics::RenderScene instance;
	return instance;
}

void Graphics::RenderScene::Add(const std::string& id, IDrawable* pDrawable)
{
	// update the identification
	pDrawable->SetId(id);

	// add to the current rendering database
	m_items.push_back(pDrawable);
}

void Graphics::RenderScene::Translate(const std::string & id, const IvVector3 & newLocation)
{
	Graphics::IDrawable* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetLocation(newLocation);
	}
}

void Graphics::RenderScene::Rotate(const std::string & id, const IvQuat & newLocation)
{
	Graphics::IDrawable* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetRotation(newLocation);
	}
}

void Graphics::RenderScene::Scale(const std::string & id, const IvVector3 & newScale)
{
	Graphics::IDrawable* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetScale(newScale);
	}
}

void Graphics::RenderScene::CastShadow(const std::string & id, bool hasShadow)
{
	Graphics::IDrawable* pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetHasShadow(hasShadow);
	}
}

void Graphics::RenderScene::Remove(const std::string & id)
{
	Graphics::IDrawable* pObj = find(id);
	if (pObj != nullptr)
	{
		m_items.remove(pObj);
		delete pObj;
	}
}

void Graphics::RenderScene::Render(float dt, bool isRenderingShadows) const
{
	for (list<Graphics::IDrawable*>::const_iterator it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::IDrawable* pObj = *it;
		pObj->Draw(dt, isRenderingShadows);
	}
}

Graphics::IDrawable * Graphics::RenderScene::find(const std::string & id)
{
	Graphics::IDrawable* retVal = nullptr;

	for (list<Graphics::IDrawable*>::iterator it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::IDrawable* pObj = *it;
		if (*pObj == id)
		{
			retVal = *it;
			break;
		}
	}

	return retVal;
}
