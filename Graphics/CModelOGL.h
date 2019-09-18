#ifndef _CMODELOGL_H_
#define _CMODELOGL_H_

#include "CommonTypes.h"
#include "ogltypes.h"
#include "IModel.h"

using namespace Types;

namespace Graphics
{

	class CModelOGL : public IModel
	{
	public:
		CModelOGL();
		virtual ~CModelOGL();
        
        // create based on SModelData
        virtual bool Create(const Model & modelInfo);
		virtual void Draw(cwc::glShader* shader);
		// allocate SModelData for custom objects
		virtual shared_ptr<Model> Allocate();
		// commit changes
		virtual bool Commit();

	private:
        struct SDrawData
        {
            SDrawData() : m_vertexArrayObject(0U), m_indicesCount(0U) {}
            SDrawData(UInt32 vao, UInt32 indCount, vector<SModelTexture> vTextures) : m_vertexArrayObject(vao), m_indicesCount(indCount), m_textures(vTextures) {}
        
			vector<SModelTexture>   m_textures;
            UInt32					m_vertexArrayObject;
            UInt32					m_indicesCount;
        };

		// copy operations
		CModelOGL(const CModelOGL& other);
		CModelOGL& operator=(const CModelOGL& other);

		bool                    m_vboBufferCreated;
        vector<UInt32>          m_vertexBufferObject;
        vector<UInt32>          m_elementBufferObject;
        vector<SDrawData>       m_drawAttr;        

	};


}
#endif // _CMODELOGL_H_