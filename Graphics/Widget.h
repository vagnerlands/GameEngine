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
        Widget* m_pChild = nullptr;
	public:
        virtual ~Widget() {}
        virtual void Draw(Float dt) = 0;
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