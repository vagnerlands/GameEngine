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

void Graphics::RenderScene::Update(const std::string & id, const IvVector3 & newLocation)
{
	for (list<Graphics::IDrawable*>::iterator it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::IDrawable* pObj = *it;
		if (*pObj == id)
		{
			pObj->SetLocation(newLocation);
			break;
		}
	}
}

void Graphics::RenderScene::Remove(const std::string & id)
{
	for (list<Graphics::IDrawable*>::iterator it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::IDrawable* pObj = *it;
		if (*pObj == id)
		{
			m_items.remove(pObj);
			delete pObj;
			break;
		}
	}
}

void Graphics::RenderScene::Render() const
{
	for (list<Graphics::IDrawable*>::const_iterator it = m_items.begin(); it != m_items.end(); it++)
	{
		Graphics::IDrawable* pObj = *it;
		pObj->Draw();
	}
}
