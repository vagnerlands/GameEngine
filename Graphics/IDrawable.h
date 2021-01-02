#ifndef _IDrawable_H_
#define _IDrawable_H_

#include "CommonTypes.h"
#include "IvVector3.h"
#include "IvQuat.h"
#include "Shaders/glsl.h"

using namespace Types;

namespace Graphics
{	
	// Everything that can be rendered shall be a "IDrawable"
	class IDrawable
	{
	public:
		IDrawable()
		{

		}
		// virtual destructor
		virtual ~IDrawable()
		{

		}
		virtual void Update(float dt) = 0;
		// render this object as is
		virtual void Draw(const class SceneItem& si, float dt, bool isRenderingShadows) = 0;

	private:
		// copy operations
		IDrawable(const IDrawable& other) = delete;
		IDrawable& operator=(const IDrawable& other) = delete;
	};


}
#endif // _IDrawable_H_