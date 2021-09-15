#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include "CommonTypes.h"
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
                _posX(rhs._posX),
                _posY(rhs._posY),
                _scale(rhs._scale),
                _color(rhs._color) {}
            explicit TextEntry(const std::string& text, Float posX, Float posY, Float scale, IvVector3 color) :
                _text(text),
                _posX(posX),
                _posY(posY),
                _scale(scale),
                _color(color) {}
            TextEntry(const TextEntry&& rhs) :
                _text(std::move(rhs._text)),
                _posX(rhs._posX),
                _posY(rhs._posY),
                _scale(rhs._scale),
                _color(rhs._color) {}
            TextEntry& operator=(const TextEntry&& rhs)
            {
                if (this != &rhs)
                {
                    _text = std::move(rhs._text);
                    _posX = rhs._posX;
                    _posY = rhs._posY;
                    _scale = rhs._scale;
                    _color = rhs._color;
                }
                return *this;
            }

            std::string _text;
            Float _posX = 0.f;
            Float _posY = 0.f;
            Float _scale = 1.f;            
            IvVector3 _color = IvVector3(1.f, 1.f, 1.f);
            bool _isFixed = false;
        };
        std::unordered_map<std::string, TextEntry> _texts;
        static TextRenderer* pInstance;
        virtual void Setup() = 0;
    public:
        virtual ~TextRenderer() {}        
        virtual void Add(const std::string& id, const std::string& text, Float posX, Float posY, Float scale, IvVector3 color)
        {
            if (_texts.find(id) != _texts.end())
            {
                _texts[id] = TextEntry(text, posX, posY, scale, color);
            }
            else
            {
                _texts.insert({ id, TextEntry(text, posX, posY, scale, color) });
            }
        }
        virtual void Remove(const std::string& id)
        {
            if (_texts.find(id) != _texts.end())
            {
                _texts.erase(id);
            }
        }
        virtual void Render() = 0;
        static TextRenderer& Instance();
    };


}
#endif // _TEXTRENDERER_H_