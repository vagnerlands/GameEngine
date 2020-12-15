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

inline const IvVector3& Graphics::SceneItem::GetLocation() const
{
	return m_location;
}

inline const IvVector3& Graphics::SceneItem::GetScale() const
{
	return m_scale;
}

inline const IvQuat& Graphics::SceneItem::GetRotation() const
{
	return m_rotation;
}

inline const std::string& Graphics::SceneItem::GetId() const
{
	return m_sceneItemId;
}

inline const bool Graphics::SceneItem::HasShadows() const
{
	return m_hasShadow;
}

inline void Graphics::SceneItem::SetLocation(const IvVector3& newLocation)
{
	m_location = newLocation;
}

inline void Graphics::SceneItem::SetScale(const IvVector3& newDimension)
{
	m_scale = newDimension;
}

inline void Graphics::SceneItem::SetRotation(const IvQuat& newRotation)
{
	m_rotation = newRotation;
}

inline void Graphics::SceneItem::SetId(const std::string& id)
{
	m_sceneItemId = id;
}

inline void Graphics::SceneItem::SetCastShadows(bool castShadows)
{
	m_hasShadow = castShadows;
}
