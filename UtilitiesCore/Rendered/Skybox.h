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

		virtual void Draw();

	protected:
		Types::SModelMesh buildMesh(
			const glm::vec3& v1, 
			const glm::vec3& v2,
			const glm::vec3& v3, 
			const glm::vec3& v4, 
			const glm::vec2& uv1,
			const glm::vec2& uv2,
			const glm::vec2& uv3,
			const glm::vec2& uv4,
			const std::string& texture);
		// generated sky model
		Graphics::IDrawable* m_skyModel;

	};

}

#endif // _Skybox_H_
