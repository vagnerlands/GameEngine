//===============================================================================
// @ Skybox
// Holds implementation of a cube with 6 textured faces to represent the sky
//===============================================================================

#ifndef _Skybox_H_
#define _Skybox_H_

#include "CommonTypes.h"
#include "OGLTypes.h"
#include "IDrawable.h"
#include <vector>

using namespace Types;

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{
	class Skybox : public Graphics::IDrawable
	{
	public:
		Skybox(const std::string& id, const vector<std::string>& faces);
		virtual ~Skybox();

		virtual void Draw(float dt, bool isRenderingShadows);
		virtual void Tick(float delta_time);

	protected:
		// generated sky model
		Graphics::IDrawable* m_skyModel;

	};

}

#endif // _Skybox_H_
