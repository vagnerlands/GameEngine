#ifndef _Widget_H_
#define _Widget_H_

#include "IDrawable.h"

using namespace Types;
namespace Graphics
{
    // interface to render 2d objects on the screen hud
	class Widget 
	{
	public:
        virtual ~Widget() {}
        virtual void Draw(Float dt) = 0;
	};
}
#endif // _Widget_H_