#include "TextRenderer.h"

Graphics::TextRenderer* Graphics::TextRenderer::pInstance = nullptr;

Graphics::TextRenderer& Graphics::TextRenderer::Instance()
{
    return *pInstance;
}