#ifndef _ModelFactory_H_
#define _ModelFactory_H_

#include "CommonTypes.h"
#include <memory>
#include "IModel.h"

using namespace Types;

namespace Graphics
{
	// Specific API for model rendering
	class ModelFactory 
	{
	public:		
        ModelFactory() { }
        static ModelFactory& Instance();
		virtual shared_ptr<Graphics::IModel> Create(const std::string& id) = 0;

    protected:
        static ModelFactory* pInstance;

	private:
		// copy operations
        ModelFactory(const ModelFactory & other) = delete;
        ModelFactory & operator=(const ModelFactory & other) = delete;        
	};
}
#endif // _ModelFactory_H_
