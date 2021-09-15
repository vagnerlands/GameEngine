#ifndef _TextArea2d_H_
#define _TextArea2d_H_

#include "Widget.h"
#include "IvVector2.h"
#include "IvVector4.h"
#include "IvMatrix44.h"
using namespace Types;

namespace Graphics
{
    // base class to render a 2D Window widget
    class TextArea2D : public Widget
    {
    protected:
        IvVector4   m_backgroundColor = { 0.f, 0.f, 0.f, 0.f}; // includes transparency
        IvVector3   m_foregroundColor = { 1.f, 1.f, 1.f }; // includes transparency
        Float       m_scale = 1.f;
        UInt32      m_uid;
        static UInt32 s_UID_Generator;
        list<std::string> m_memo;
        bool m_isLru = true; // implements least recent used (keep track of the list depth, when it hits the maximum, removes the oldest entry)
        UInt32 m_maxDepth = 35U;
        UInt32 m_maxStrLen = 32U;
        virtual void draw(Float dt) = 0; // to be implemented in DirectX, OpenGL, Vulkan ...
    public:
        explicit TextArea2D(const IvVector4& background_color, const IvVector3& foreground_color, Float scale);
        explicit TextArea2D(const IvVector2& location, const IvVector2& size, const IvVector4& background_color, const IvVector3& foreground_color);
        virtual ~TextArea2D();

        void AddLine(const std::string& line);
        void Clear();

        virtual void Draw(Float dt) override;
    };

}
#endif // _Window2D_H_