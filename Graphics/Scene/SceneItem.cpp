#include "SceneItem.h"

void Graphics::SceneItem::Render(float dt, bool isRenderingShadows) const
{
	m_pDrawable->Draw(*this, dt, isRenderingShadows);
}

void Graphics::SceneItem::ReplaceDrawable(Graphics::IDrawable* pDrawable)
{
	m_pDrawable = pDrawable;
}

void Graphics::SceneItem::Release()
{
	if (m_pDrawable != nullptr)
	{
		delete m_pDrawable;
	}
}