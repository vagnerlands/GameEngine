#include "Window2D.h"

void Graphics::Window2D::updateModel()
{
    // translate model
    IvMatrix44 translateModel;
    translateModel.Identity();
    // rotate model
    IvMatrix44 rotateModel;
    rotateModel.Identity();
    // scale model
    IvMatrix44 scaleModel;
    scaleModel.Identity();
    // calculates idependently each transformation
    // rotate transformation
    //rotateModel.Rotation(m_rota);
    // scale transformation
    scaleModel.Scaling(IvVector3(m_size.GetX(), m_size.GetY(), 1.f));
    // translation transformation
    translateModel.Translation(IvVector3(m_location.GetX(), m_location.GetY(), 0.f));
    // combine both transformations
    m_model = translateModel * scaleModel * rotateModel;
}

Graphics::Window2D::Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color) :
    m_location(location),
    m_size(size),
    m_color(color)
{}

Graphics::Window2D::Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color, const std::string& textureFile, const std::string& textureType) :
    m_location(location),
    m_size(size),
    m_color(color),
    m_texture{textureFile, textureType}
{}

Graphics::Window2D::~Window2D()
{

}

void Graphics::Window2D::Draw(Float dt)
{

    draw(dt);
}
