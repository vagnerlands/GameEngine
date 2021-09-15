#ifndef _Widget_H_
#define _Widget_H_

#include "WidgetTransforms.h"

using namespace Types;
namespace Graphics
{
    // interface to render 2d objects on the screen hud
	class Widget : public WidgetTransforms
	{
    protected:
        std::string m_id;
        list<std::shared_ptr<Widget>> m_children;
	public:
        enum eFillStyle
        {
            eFillStyle_None = 0,
            eFillStyle_Fill
        };

        Widget() {}
        virtual ~Widget() {}
        virtual void Setup() = 0; // allocate buffers, textures and everything else this widget needs to work for each platform
        virtual void Draw(Float dt) = 0; // implements the rendering for each platform
        virtual void SetId(const std::string& id) { m_id = id; }
        virtual const std::string& Id() const { return m_id; }
        void Append(std::shared_ptr<Widget> pChild, eFillStyle fill = eFillStyle_None)
        {
            if (pChild != nullptr)
            { 
                m_children.push_back(pChild);
                pChild->SetParent(this); // for transformation purposes.
                if (eFillStyle_Fill == fill)
                {
                    pChild->SetSize(m_size);
                }
            }
        }
        void DrawChildren(Float dt)
        {
            for (std::shared_ptr<Widget> child : m_children)
            {
                child->Draw(dt);
            }
        }
	};
}
#endif // _Widget_H_