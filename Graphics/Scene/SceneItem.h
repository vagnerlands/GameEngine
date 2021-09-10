#ifndef _SceneItem_H_
#define _SceneItem_H_

#include "CommonTypes.h"
#include "IDrawable.h"
#include "IvVector2.h"
#include "IvVector3.h"
#include "IvMatrix44.h"
#include "IvQuat.h"

// TODO: wrap this
#include "glsl.h"

#include <list>

using namespace Types;

namespace Graphics
{
	// Everything that can be rendered shall be a "IDrawable"
	class SceneItem 
	{
	public:
		// virtual ctor
		SceneItem(const std::string& id, shared_ptr<Graphics::IDrawable> pDrawable);
		// virtual dtor
		virtual ~SceneItem()
		{}

        // grooby style, allows caller to append more transformations
        virtual SceneItem& SetLocation(const IvVector3& newLocation);
        virtual SceneItem& SetScale(const IvVector3& newDimension);
        virtual SceneItem& SetRotation(const IvQuat& newRotation);
        virtual SceneItem& SetTextureUV(const IvVector2& uvFactor);
        virtual SceneItem& DisplayBoundingBox(bool display);
        // end of groovy

        virtual void SetUpScene(cwc::glShader* pShader) const;
        virtual void SetUpAnimation(cwc::glShader* pShader) const;
        virtual void SetUpShadows(cwc::glShader* pShader) const;
        virtual void ShadowsPass() const;
        // visitor - base implementation do nothing
        virtual void Query(class SceneQuery& query)
        {
            // do nothing
        }

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

		void Render(float dt, bool isRenderingShadows = false) const;
		void ReplaceDrawable(shared_ptr<Graphics::IDrawable> pDrawable);
		void Release();

		const IvMatrix44& GetModel() const;
		const std::string& GetId() const;
		const bool HasShadows() const;

		void SetId(const std::string& id);
		void SetCastShadows(bool castShadows);

        const IvVector3& GetLocation() const;
        const IvVector3& GetScale() const;
        const IvQuat& GetRotation() const;
        const IvVector2& GetUVFactor() const;
        IvAABB GetBoundingBox() const;

        const shared_ptr<Graphics::IDrawable> GetDrawable() const;

	protected:
		// copy operations
		SceneItem(const SceneItem& other) = delete;
		SceneItem& operator=(const SceneItem& other) = delete;
        
        void updateModel();

		// model to be rendered
        shared_ptr<Graphics::IDrawable> m_pDrawable;
		// identification
		std::string		m_sceneItemId;
		// world location of this drawable
		IvVector3		m_location;
		// scale resize for the model
		IvVector3		m_scale;
		// rotation matrix for the model
		IvQuat			m_rotation;
		// texture resize, default value is 1x1
		IvVector2		m_uvFactor;
		// pre-calculated model
		IvMatrix44		m_model;
		// does this item cast a shadow
		bool			m_hasShadow;
        // bounding box of the inner drawable + transformations
        const IvAABB*   m_pBoundingBox;
        // shall display a bounding box around the Drawable
        std::shared_ptr<SceneItem>  m_nextScene = nullptr;
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

inline const IvVector2& Graphics::SceneItem::GetUVFactor() const
{
	return m_uvFactor;
}

inline const IvMatrix44& Graphics::SceneItem::GetModel() const
{
	return m_model;
}

inline const std::string& Graphics::SceneItem::GetId() const
{
	return m_sceneItemId;
}

inline const bool Graphics::SceneItem::HasShadows() const
{
	return m_hasShadow;
}

inline Graphics::SceneItem& Graphics::SceneItem::SetLocation(const IvVector3& newLocation)
{
	m_location = newLocation;
    if (m_nextScene != nullptr)
        (*m_nextScene).SetLocation(newLocation);
	updateModel();
    return *this;
}

inline Graphics::SceneItem& Graphics::SceneItem::SetScale(const IvVector3& newDimension)
{
	m_scale = newDimension;
    if (m_nextScene != nullptr)
        (*m_nextScene).SetScale(newDimension);
	updateModel();
    return *this;
}

inline Graphics::SceneItem& Graphics::SceneItem::SetRotation(const IvQuat& newRotation)
{
	m_rotation = newRotation;
    if (m_nextScene != nullptr)
        (*m_nextScene).SetRotation(newRotation);
	updateModel();
    return *this;
}

inline Graphics::SceneItem& Graphics::SceneItem::SetTextureUV(const IvVector2& uvFactor)
{
	m_uvFactor = uvFactor;
    return *this;
}

inline void Graphics::SceneItem::SetId(const std::string& id)
{
	m_sceneItemId = id;
}

inline void Graphics::SceneItem::SetCastShadows(bool castShadows)
{
	m_hasShadow = castShadows;
}

inline const shared_ptr<Graphics::IDrawable> Graphics::SceneItem::GetDrawable() const
{
    return m_pDrawable;
}

#endif // _SceneItem_H_
