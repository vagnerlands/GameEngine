#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include "CommonTypes.h"
#include "IvVector3.h"

using namespace Types;

namespace Graphics
{
    class TextRenderer
    {
    protected:
        static TextRenderer* pInstance;
        virtual void Setup() = 0;
    public:
        virtual ~TextRenderer() {}        
        virtual void Render(const std::string& text, Float posX, Float posY, Float scale, IvVector3 color) = 0;
        static TextRenderer& Instance();
    };


}
#endif // _TEXTRENDERER_H_