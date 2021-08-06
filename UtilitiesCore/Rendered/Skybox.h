//===============================================================================
// @ Skybox
// Holds implementation of a cube with 6 textured faces to represent the sky
//===============================================================================

#ifndef _Skybox_H_
#define _Skybox_H_

#include "CommonTypes.h"
#include "OGLTypes.h"
#include "IDrawable.h"
#include "SceneItem.h"

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
        static shared_ptr<Skybox> CreateSky(const std::string& id, const vector<std::string>& faces);
		virtual ~Skybox();

		virtual void Update(float dt) override;
		virtual void Draw(const class Graphics::SceneItem& si, float dt, bool isRenderingShadows) override;
		virtual void Tick(float delta_time);

	protected:
		// generated sky model
		Graphics::SceneItem m_skyModel;

	};

}

#endif // _Skybox_H_
