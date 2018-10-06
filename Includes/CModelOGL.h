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
		virtual bool Create(const SModelData& modelInfo);
		virtual void Draw();
		// allocate SModelData for custom objects
		virtual SModelData& Allocate();
		// commit changes
		virtual bool Commit();

	private:
		// copy operations
		CModelOGL(const CModelOGL& other);
		CModelOGL& operator=(const CModelOGL& other);

		UInt32 m_elementBuffer[VertexBuffer_Max_Num];
		bool m_vboBufferCreated;
		UInt32 m_vertexArrayObject;

		UInt32 m_numberOfIndexes;
	};


}
#endif // _CMODELOGL_H_