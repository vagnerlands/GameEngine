#ifndef _IMODEL_H_
#define _IMODEL_H_

#include "CommonTypes.h"
#include <memory>
#include "Model.h"
#include "IDrawable.h"

using namespace Types;

namespace Graphics
{
	// Specific API for model rendering
	class IModel : public IDrawable
	{
	public:
		IModel();
		virtual ~IModel() { }
        virtual bool Create() = 0;
		virtual bool Apply(const Model* pModelInfo) = 0;
		virtual shared_ptr<Model> Allocate() = 0;
		virtual bool Commit() = 0;        
        virtual void SetWireMode(bool display) = 0;
        virtual bool SetTexture(eTextures texture, const char* pTextureFile = "__no_valid_texture__.bmp") = 0;
	protected:
		// data to be used, this shall be released once Commit() is called
        shared_ptr<Model>	m_pData;
		// for debug purposes
		std::string			m_modelName;

	private:
		// copy operations
		IModel(const IModel& other) = delete;
		IModel& operator=(const IModel& other) = delete;
	};


}
#endif // _IMODEL_H_