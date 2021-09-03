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
        Widget* m_pChild = nullptr;
	public:
        Widget() {}
        virtual ~Widget() {}
        virtual void Setup() = 0; // allocate buffers, textures and everything else this widget needs to work for each platform
        virtual void Draw(Float dt) = 0; // implements the rendering for each platform
        virtual void SetId(const std::string& id) { m_id = id; }
        virtual const std::string& Id() const { return m_id; }
        void Append(Widget* pChild)
        {
            if (pChild != nullptr)
            { 
                m_pChild = pChild;
                m_pChild->SetParent(this); // for transformation purposes.
            }
        }
	};
}
#endif // _Widget_H_