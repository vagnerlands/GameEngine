#include "Factory/ModelFactoryOGL.h"
#include "OpenGl/CModelOGL.h"

void Graphics::ModelFactoryOGL::Initialize() 
{
    Graphics::ModelFactory::pInstance = new Graphics::ModelFactoryOGL;
}

std::shared_ptr<Graphics::IModel> Graphics::ModelFactoryOGL::Create(const std::string& id)
{
    return std::make_shared<Graphics::CModelOGL>(id);
}

void Graphics::ModelFactoryOGL::Destroy()
{
    if (Graphics::ModelFactory::pInstance != nullptr)
    {
        delete Graphics::ModelFactory::pInstance;
    }
}