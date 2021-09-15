#include "TextArea2D.h"
#include "TextRenderer.h"

UInt32 Graphics::TextArea2D::s_UID_Generator = 0;

Graphics::TextArea2D::TextArea2D(const IvVector4 & background_color, const IvVector3& foreground_color, Float scale) :
    m_backgroundColor(background_color),
    m_foregroundColor(foreground_color),
    m_scale(scale),
    m_uid(s_UID_Generator++)
{
}

Graphics::TextArea2D::TextArea2D(const IvVector2 & location, const IvVector2 & size, const IvVector4 & background_color, const IvVector3 & foreground_color) :
    m_backgroundColor(background_color),
    m_foregroundColor(foreground_color),
    m_uid(s_UID_Generator++)
{
    m_location = location;
    m_size = size;
}

Graphics::TextArea2D::~TextArea2D()
{
    TextRenderer::Instance().Remove("TextArea_" + to_string(m_uid));
}

void Graphics::TextArea2D::AddLine(const std::string & line)
{
    std::string entry = line;
    if (entry.length() > m_maxStrLen)
    {
        entry = entry.substr(0, m_maxStrLen);
    }

    if (m_memo.size() + 1 > m_maxDepth)
    {
        m_memo.pop_back();
    }

    m_memo.push_front(entry);
}

void Graphics::TextArea2D::Clear()
{
    m_memo.clear();
}

void Graphics::TextArea2D::Draw(Float dt)
{
    draw(dt); // calls concrete implementation (OpengGL, DirectX, or any other)
    string logtext;
    // append all strings to the logger space
    for (const auto& s : m_memo)
    {
        logtext += s;
        logtext += "\n";
    }
    IvVector2 transformed_location(m_location.GetX() + m_transforms.GetFloatPtr()[12], m_location.GetX() + m_transforms.GetFloatPtr()[13]);
    TextRenderer::Instance().Add("TextArea_" + to_string(m_uid), logtext, transformed_location.GetX(), transformed_location.GetY(), m_scale, m_foregroundColor);
    Widget::DrawChildren(dt);
}
