#include "TextRenderer.h"

Graphics::TextRenderer* Graphics::TextRenderer::pInstance = nullptr;

Graphics::TextRenderer& Graphics::TextRenderer::Instance()
{
    return *pInstance;
}

Graphics::TextRenderer::~TextRenderer() {}

void Graphics::TextRenderer::Add(const std::string& id, const std::string& text, const IvVector2& position, const IvVector2& area, Float scale, const IvVector3& color)
{
    if (_texts.find(id) != _texts.end())
    {
        _texts[id] = TextEntry(text, position, area, scale, color);
    }
    else
    {
        _texts.insert({ id, TextEntry(text, position, area, scale, color) });
    }
}

void Graphics::TextRenderer::Add(const std::string& id, const std::string& text, const IvVector2& position, Float scale, const IvVector3& color)
{
    if (_texts.find(id) != _texts.end())
    {
        _texts[id] = TextEntry(text, position, IvVector2(9999.f, 0.f), scale, color);
    }
    else
    {
        _texts.insert({ id, TextEntry(text, position, IvVector2(9999.f, 0.f), scale, color) });
    }
}

void Graphics::TextRenderer::Remove(const std::string& id)
{
    if (_texts.find(id) != _texts.end())
    {
        _texts.erase(id);
    }
}