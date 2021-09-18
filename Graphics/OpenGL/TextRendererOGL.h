#ifndef _TextRendererOgl_H_
#define _TextRendererOgl_H_

#include "TextRenderer.h"
#include "glsl.h"
#include "IvVector2.h"
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H  

using namespace Types;

namespace Graphics
{
    struct Character 
    {
        unsigned int TextureID;  // ID handle of the glyph texture
        IvVector2    Size;       // Size of glyph
        IvVector2    Bearing;    // Offset from baseline to left/top of glyph
        IvVector2    TextCoordsS; // X axis Texture coordinates (mapping)
        IvVector2    TextCoordsT; // Y axis Texture coordinates (mapping)
        unsigned int Advance;    // Offset to advance to next glyph
    };    

    class TextRendererOGL : public TextRenderer
    {
        std::map<char, Character> Characters;
        unsigned int VAO, VBO;
        static const std::string    s_shaderName;
        static cwc::glShader*       s_pShader;
        virtual void Setup() override;
    public:
        static void Create();
        ~TextRendererOGL();        
        virtual void Render() override;
    };


}
#endif // _TextRendererOgl_H_