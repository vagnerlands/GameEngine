#ifndef _RenderUI_H_
#define _RenderUI_H_

#include "CommonTypes.h"
#include "IvVector2.h"
#include <map>

using namespace Types;

namespace Graphics
{
    class Widget;
	// Everything that can be rendered shall be a "IDrawable"
	class RenderUI
	{
	public:
		// virtual destructor
		virtual ~RenderUI()
		{

		}

		static RenderUI& Instance();

		virtual shared_ptr<Graphics::Widget> Add(const std::string& id, shared_ptr<Graphics::Widget> pObject);

        virtual void Remove(const std::string & id);

		// Update ilumination location
		virtual void Translate(const std::string& id, const IvVector2& newLocation);

		// Update ilumination location
		//virtual void Rotate(const std::string& id, const IvQuat& newLocation);

		// Update ilumination location
		virtual void Resize(const std::string& id, const IvVector2& newSize);

		// Update ilumination location
		virtual void SetTextureUV(const std::string& id, const IvVector2& uvFactor);

        virtual void Render(Float dt) const;

        virtual void Destroy();

	protected:
		typedef std::map<std::string, shared_ptr<Graphics::Widget>> DatabaseUIType;
        DatabaseUIType m_items;

        RenderUI()
		{

		}

        shared_ptr<Graphics::Widget> find(const std::string & id);

	private:
		// copy operations
        RenderUI(const RenderUI& other) = delete;
        RenderUI& operator=(const RenderUI& other) = delete;
	};


}
#endif // _RenderScene_H_