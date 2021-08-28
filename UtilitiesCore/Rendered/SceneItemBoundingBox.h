//===============================================================================
// @ Skybox
// Holds implementation of a cube with 6 textured faces to represent the sky
//===============================================================================

#ifndef _SceneItemBoundingBox_H_
#define _SceneItemBoundingBox_H_

#include "CommonTypes.h"
#include "SceneItem.h"

#include <vector>

using namespace Types;

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{
	class SceneItemBoundingBox : public Graphics::SceneItem
	{
    public:
        explicit SceneItemBoundingBox(const Graphics::SceneItem& mesh);
        static std::shared_ptr<SceneItemBoundingBox> CreateBoundingBox(const Graphics::SceneItem& mesh);
		virtual ~SceneItemBoundingBox();
 
	protected:
		// generated sky model
		const Graphics::SceneItem& mMeshReference;
        // model to be rendered
        Graphics::SceneItem m_sceneItem;

	};

}

#endif // _SceneItemBoundingBox_H_
