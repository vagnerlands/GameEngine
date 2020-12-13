#ifndef _SceneItem_H_
#define _SceneItem_H_

#include "CommonTypes.h"
#include "IDrawable.h"
#include "IvVector3.h"
#include "IvQuat.h"
#include <list>

using namespace Types;

namespace Graphics
{
	// Everything that can be rendered shall be a "IDrawable"
	class SceneItem
	{
	public:
		// virtual ctor
		SceneItem(const std::string id, Graphics::IDrawable* pDrawable) :
			m_pDrawable(pDrawable),
			m_sceneItemId(id),
			m_location(0.f, 0.f, 0.f), // default location
			m_scale(1.f, 1.f, 1.f), // default scale (original size from editor)
			m_hasShadow(true)
		{
			m_rotation.Identity();
		}
		// virtual dtor
		~SceneItem()
		{
		}

		void Render(float dt, bool isRenderingShadows = false) const;
		void ReplaceDrawable(Graphics::IDrawable* pDrawable);
		void Release();

		const IvVector3& GetLocation() const;
		const IvVector3& GetScale() const;
		const IvQuat& GetRotation() const;
		const std::string& GetId() const;
		const bool HasShadows() const;
		void SetLocation(const IvVector3& newLocation);
		void SetScale(const IvVector3& newDimension);
		virtual void SetRotation(const IvQuat& newRotation);
		void SetId(const std::string& id);
		void SetCastShadows(bool castShadows);

		// for comparison
		virtual bool operator==(const SceneItem& other)
		{
			return other.m_sceneItemId == m_sceneItemId;
		}
		// for comparison
		virtual bool operator==(const std::string& other)
		{
			return other == m_sceneItemId;
		}


	private:
		// copy operations
		SceneItem(const SceneItem& other) = delete;
		SceneItem& operator=(const SceneItem& other) = delete;

		// model to be rendered
		Graphics::IDrawable* m_pDrawable;
		// identification
		std::string		m_sceneItemId;
		// world location of this drawable
		IvVector3		m_location;
		// scale resize for the model
		IvVector3		m_scale;
		// rotation matrix for the model
		IvQuat			m_rotation;
		// does this item cast a shadow
		bool			m_hasShadow;
	};
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


#endif // _SceneItem_H_