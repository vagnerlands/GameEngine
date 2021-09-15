#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include "CommonTypes.h"
#include "IvVector2.h"
#include "IvVector3.h"
#include <list>

using namespace Types;

namespace Graphics
{
    class TextRenderer
    {
    protected:
        struct TextEntry
        {
            TextEntry() {}
            TextEntry(const TextEntry& rhs) :
                _text(rhs._text),
                _position(rhs._position),
                _area(rhs._area),
                _scale(rhs._scale),
                _color(rhs._color) {}

            explicit TextEntry(const std::string& text, const IvVector2& position, const IvVector2& area, Float scale, const IvVector3& color) :
                _text(text),
                _position(position),
                _area(area),
                _scale(scale),
                _color(color) {}

            TextEntry(const TextEntry&& rhs) :
                _text(std::move(rhs._text)),
                _position(rhs._position),
                _area(rhs._area),
                _scale(rhs._scale),
                _color(rhs._color) {}

            TextEntry& operator=(const TextEntry&& rhs)
            {
                if (this != &rhs)
                {
                    _text = std::move(rhs._text);
                    _position = rhs._position;
                    _area = rhs._area;
                    _scale = rhs._scale;
                    _color = rhs._color;
                }
                return *this;
            }

            std::string _text;
            IvVector2 _position = IvVector2(0.f, 0.f);
            IvVector2 _area = IvVector2(0.f, 0.f);
            Float _scale = 1.f;            
            IvVector3 _color = IvVector3(1.f, 1.f, 1.f);
        };
        std::unordered_map<std::string, TextEntry> _texts;
        static TextRenderer* pInstance;
        virtual void Setup() = 0;
    public:
        virtual ~TextRenderer();
        virtual void Add(const std::string& id, const std::string& text, const IvVector2& position, const IvVector2& area, Float scale, const IvVector3& color);

        virtual void Add(const std::string& id, const std::string& text, const IvVector2& position, Float scale, const IvVector3& color);
        
        virtual void Remove(const std::string& id);

        virtual void Render() = 0;
        static TextRenderer& Instance();
    };


}
#endif // _TEXTRENDERER_H_