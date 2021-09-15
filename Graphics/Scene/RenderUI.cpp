#include "RenderUI.h"
#include "Widget.h"
#include "TextRenderer.h"

Graphics::RenderUI & Graphics::RenderUI::Instance()
{
	static Graphics::RenderUI instance;
	return instance;
}

shared_ptr<Graphics::Widget> 
Graphics::RenderUI::Add(const std::string& id, shared_ptr<Graphics::Widget> pObject)
{	
	// add to the current rendering database
	m_items.insert(std::make_pair(id, pObject));
    pObject->Setup();
    return pObject;
}

void Graphics::RenderUI::Translate(const std::string & id, const IvVector2 & newLocation)
{
    shared_ptr<Graphics::Widget> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetLocation(newLocation);
	}
}

//void Graphics::RenderUI::Rotate(const std::string & id, const IvQuat & newLocation)
//{
//    shared_ptr<Graphics::Widget> pObj = find(id);
//	if (pObj != nullptr)
//	{
//		pObj->SetRotation(newLocation);
//	}
//}

void Graphics::RenderUI::Resize(const std::string & id, const IvVector2 & newSize)
{
    shared_ptr<Graphics::Widget> pObj = find(id);
	if (pObj != nullptr)
	{
		pObj->SetSize(newSize);
	}
}

void Graphics::RenderUI::SetTextureUV(const std::string& id, const IvVector2& uvFactor)
{

}

void Graphics::RenderUI::Remove(const std::string & id)
{
    shared_ptr <Graphics::Widget> pObj = find(id);
	if (pObj != nullptr)
	{
        m_items.erase(id);
        pObj = nullptr;
	}
}

void Graphics::RenderUI::Render(float dt) const
{
	for (auto& it = m_items.begin(); it != m_items.end(); it++)
	{
        (*it).second->Draw(dt);
	}
    // render all texts
    TextRenderer::Instance().Render();
}

shared_ptr<Graphics::Widget> Graphics::RenderUI::find(const std::string & id)
{
    auto ret = m_items.find(id);
    if (ret == m_items.end())
        return nullptr;
    return ret->second;
}

void Graphics::RenderUI::Destroy()
{
    m_items = DatabaseUIType();
}
