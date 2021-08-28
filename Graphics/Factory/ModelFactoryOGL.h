#ifndef _ModelFactoryOGL_H_
#define _ModelFactoryOGL_H_

#include "Factory/ModelFactory.h"

namespace Graphics
{
	// Specific API for model rendering
    class ModelFactoryOGL : public ModelFactory
	{
	public:		
        static void Initialize();
        static void Destroy();
		virtual std::shared_ptr<Graphics::IModel> Create(const std::string& id) override;

	private:
        ModelFactoryOGL() { }
		// copy operations
        ModelFactoryOGL(const ModelFactoryOGL& other) = delete;
        ModelFactoryOGL& operator=(const ModelFactoryOGL& other) = delete;
	};


}
#endif // _ModelFactoryOGL_H_