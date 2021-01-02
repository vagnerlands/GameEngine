#ifndef _RenderScene_H_
#define _RenderScene_H_

#include "CommonTypes.h"
#include "IDrawable.h"
#include "IvVector3.h"
#include "IvQuat.h"
#include "SceneItem.h"
#include <list>

using namespace Types;

namespace Graphics
{
	// Everything that can be rendered shall be a "IDrawable"
	class RenderScene 
	{
	public:
		// virtual destructor
		virtual ~RenderScene()
		{

		}

		static RenderScene& Instance();

		virtual void Add(const std::string& id, IDrawable* pDrawable, eSceneItemType type);

		// replace the current model by another model
		virtual void Change(const std::string& id, IDrawable* pDrawable);

		// Update ilumination location
		virtual void Translate(const std::string& id, const IvVector3& newLocation);

		// Update ilumination location
		virtual void Rotate(const std::string& id, const IvQuat& newLocation);

		// Update ilumination location
		virtual void Scale(const std::string& id, const IvVector3& newScale);

		// Update ilumination location
		virtual void SetTextureUV(const std::string& id, const IvVector2& uvFactor);

		// Update ilumination location
		virtual void CastShadow(const std::string& id, bool hasShadow);

		// Update ilumination location
		virtual void Remove(const std::string& id);
		

		//virtual void Update(float dt) const;

		virtual void Render(float dt, bool isRenderingShadows = false) const;

	protected:
		//list<Graphics::IDrawable*> m_items;
		typedef list<Graphics::SceneItem*> DatabaseSceneItemsType;
		DatabaseSceneItemsType m_items;

		RenderScene()
		{

		}

		Graphics::SceneItem* find(const std::string& id);

	private:
		// copy operations
		RenderScene(const RenderScene& other) = delete;
		RenderScene& operator=(const RenderScene& other) = delete;
	};


}
#endif // _RenderScene_H_