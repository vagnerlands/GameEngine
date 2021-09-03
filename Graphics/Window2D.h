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
        IvVector4 m_color = { 1.f, 1.f, 1.f, 1.f}; // includes transparency
        std::string m_texture; // file name 
        virtual void draw(Float dt) = 0; // to be implemented in DirectX, OpenGL, Vulkan ...
    public:
        explicit Window2D(const IvVector4& color, const std::string& textureFile);
        explicit Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color);
        explicit Window2D(const IvVector2& location, const IvVector2& size, const IvVector4& color, const std::string& textureFile);
        virtual ~Window2D();         
        virtual void Draw(Float dt) override;
        void SetTexture(const char* textureName) { m_texture = textureName; }
        void SetColor(const IvVector4& color) { m_color = color; }
    };

}
#endif // _Window2D_H_