#include "Window2D.h"

Graphics::Window2D::Window2D(const IvVector4& color, const std::string& textureFile) :
    m_color(color),
    m_texture(textureFile)
{}

Graphics::Window2D::Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color) :
    m_color(color)
{
    m_location = location;
    m_size = size;
}

Graphics::Window2D::Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color, const std::string& textureFile) :
    m_color(color),
    m_texture{textureFile}
{    
    m_location = location;
    m_size = size; 
}

Graphics::Window2D::~Window2D()
{}

void Graphics::Window2D::Draw(Float dt)
{
    draw(dt); // calls concrete implementation (OpengGL, DirectX, or any other)
    if (m_pChild != nullptr)
    {
        m_pChild->Draw(dt);
    }
}
