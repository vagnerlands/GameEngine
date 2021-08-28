#include "Factory/ModelFactory.h"

using namespace Graphics;

ModelFactory* ModelFactory::pInstance = nullptr;

ModelFactory& ModelFactory::Instance()
{
    return *pInstance;
}

