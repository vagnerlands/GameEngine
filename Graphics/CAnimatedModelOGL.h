#ifndef _CANIMATEDMODELOGL_H_
#define _CANIMATEDMODELOGL_H_

#include "CommonTypes.h"
#include "ogltypes.h"
#include "CModelOGL.h"

using namespace Types;

namespace Graphics
{

	class CAnimatedModelOGL : public CModelOGL
	{
	public:
		CAnimatedModelOGL(string id);
		virtual ~CAnimatedModelOGL();
        
        // create based on SModelData
        virtual bool Create(const Model & modelInfo);
		//bool SetShader(const string& shaderName);
		virtual void Draw(bool isRenderingShadows);
		// executes on every app cycle
		virtual void Tick(float delta_time);

	protected:

		// copy operations
		CAnimatedModelOGL(const CAnimatedModelOGL& other) = delete;
		CAnimatedModelOGL& operator=(const CAnimatedModelOGL& other) = delete;


	};


}
#endif // _CANIMATEDMODELOGL_H_