#ifndef _Window_H_
#define _Window_H_

#include "Widget.h"
#include "IvVector2.h"
#include "IvVector4.h"
#include "IvMatrix44.h"
using namespace Types;

namespace Graphics
{
    // base class to render a 2D Window widget
    class Window2D : public Widget
    {
    protected:
        IvMatrix44 m_model;
        IvVector2 m_location = { 500.f, 200.f };
        IvVector2 m_size{ 100.f, 100.f };
        IvVector4 m_color = { 1.f, 1.f, 1.f, 0.5f}; // includes transparency
        std::pair<std::string, std::string> m_texture; // includes transparency    
        virtual void draw(Float dt) = 0; // to be implemented in DirectX, OpenGL, Vulkan ...
        virtual void updateModel();
    public:
        Window2D() {}
        Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color);
        Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color, const std::string& textureFile, const std::string& textureType);
        virtual ~Window2D(); 
        virtual void Setup() = 0; // to be implemented in DirectX, OpenGL, Vulkan ...
        virtual void Draw(Float dt) override;
    };

}
#endif // _Window2D_H_