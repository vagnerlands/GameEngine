#include "TextRendererOGL.h"
#include <iostream>
#include "ogltypes.h"
#include "OglHelper.h"
#include "IRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <png.h>
const std::string Graphics::TextRendererOGL::s_shaderName = "text"; // default shader for a window, not thinking about specializations for now
cwc::glShader* Graphics::TextRendererOGL::s_pShader = nullptr; // 1 shader to rule them all...

void Graphics::TextRendererOGL::Create()
{
    static TextRendererOGL instance;
    TextRenderer::pInstance = &instance;
    instance.Setup();
}

Graphics::TextRendererOGL::~TextRendererOGL()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Graphics::TextRendererOGL::Setup()
{
    // generate the shader only once for all windows
    if (s_pShader == nullptr)
    {
        s_pShader = Graphics::OglHelper::generateShader(s_shaderName);
    }
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "Assets/Fonts/tahoma.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    const UInt32 cFontSize = 50U;
    const UInt32 cTextureWidth = 640;
    const UInt32 cTextureHeight = 480;
    UInt32 offsetX = 0;
    UInt32 offsetY = 0;
    UInt32 bufferOffset = 0;

    UByte FontAtlas[cTextureWidth * cTextureHeight] = { 0x00 };
    UByte cor = 0xff;
    for (unsigned char ch = 32; ch < 127; ch++)
    {
        // load character glyph 
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        const UInt32 cBufferSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
        for (Int32 r = 0; r < cFontSize; ++r)
        {
            const UInt32 cOffset = (cFontSize * offsetX) + (cFontSize * (offsetY * cTextureWidth) + (r * cTextureWidth));
            if (r < face->glyph->bitmap.rows)
            {
                memcpy(&FontAtlas[cOffset], face->glyph->bitmap.buffer + r * face->glyph->bitmap.width, face->glyph->bitmap.width);
            }
        }
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        IvVector2 coordsS((Float)((offsetX * cFontSize) / (Float)cTextureWidth), (Float)((offsetY * cFontSize) / (Float)cTextureHeight));
        IvVector2 coordsT((Float)((offsetX * cFontSize + face->glyph->bitmap.width) / (Float)cTextureWidth), (Float)((offsetY * cFontSize + face->glyph->bitmap.rows) / (Float)cTextureHeight));

        // now store character for later use
        Character character = {
            texture,
            IvVector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            IvVector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            coordsS,
            coordsT,
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(ch, character));

        ++offsetX;
        if ((offsetX * cFontSize) >= cTextureWidth)
        {
            offsetX = 0;
            offsetY++;
        }
    }

    // ENABLE THIS TO DEBUG CONTENT...
    //char* bmp_data = new char[cTextureWidth * cTextureHeight * 3];
    //for (int i = 0; i < (cTextureWidth * cTextureHeight); ++i)
    //{
    //    bmp_data[i * 3 + 0] = 0xff;
    //    bmp_data[i * 3 + 1] = FontAtlas[i];
    //    bmp_data[i * 3 + 2] = 0xff;
    //}
    //FILE* fd;
    //fopen_s(&fd, "./out.bmp", "wb");
    //UByte bmp_header[54] = { 0x42, 0x4D, 0x36, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    //fwrite(bmp_header, sizeof(bmp_header), 1, fd);
    //fwrite(bmp_data, cTextureWidth* cTextureHeight * 3, 1, fd);
    //fclose(fd);
    //delete bmp_data;


    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED, // only 1ch required
        cTextureWidth,
        cTextureHeight,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        FontAtlas
    );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Graphics::TextRendererOGL::Render()
{
    // clear gl errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, Characters[32].TextureID);
    // activate corresponding render state	
    s_pShader->Use();
    for (const auto& it : _texts)
    {
        const auto& i = it.second;
        // projection
        IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
        s_pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
        s_pShader->setUniform3f("textColor", i._color.GetX(), i._color.GetY(), i._color.GetZ());
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        const Int32 cLineSpace = (40 * i._scale);
        const Int32 cTopOffsetY = i._position.GetY() + i._area.GetY();

        // text is written from top-left to right-down
        // before writting each character, we check the limits of the text area
        // if the text won't fit in the X-axis of the text area, we advance the Y-axis
        // and reset the X-axis.
        // if the Y-axis reachs to the Y-axis text area limit, we stop writting this string and
        // move to the next string.
        Int32 line_counter = 1;
        Int32 x = i._position.GetX();
        Int32 y = cTopOffsetY - cLineSpace;
        if (y < 0)
            y = 0;

        // iterate through all characters
        std::string::const_iterator c = i._text.begin();
        for (c = i._text.begin(); c != i._text.end(); c++)
        {
            // in case there is a "new line" or the text has reached the x axis limit, 
            // we must perform a "carriage return" and "new line feed"
            if ((*c == '\n')
                || (x > i._area.GetX()))
            {
                ++line_counter;
                x = i._position.GetX();
                y = cTopOffsetY - (line_counter * cLineSpace);
                if (y < i._position.GetY()) // if next line has crossed the bottom most tolerable limit, we must stop
                {
                    break;
                }
                continue; // move to the next 
            }
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.GetX() * i._scale;
            float ypos = y - (ch.Size.GetY() - ch.Bearing.GetY()) * i._scale;

            float w = ch.Size.GetX() * i._scale;
            float h = ch.Size.GetY() * i._scale;
            // update VBO for each character
            float vertices[6][4] =
            {
                { xpos,     ypos + h,   ch.TextCoordsS.GetX(), ch.TextCoordsS.GetY() },
                { xpos,     ypos,       ch.TextCoordsS.GetX(), ch.TextCoordsT.GetY()},
                { xpos + w, ypos,       ch.TextCoordsT.GetX(), ch.TextCoordsT.GetY() },

                { xpos,     ypos + h,   ch.TextCoordsS.GetX(), ch.TextCoordsS.GetY() },
                { xpos + w, ypos,       ch.TextCoordsT.GetX(), ch.TextCoordsT.GetY() },
                { xpos + w, ypos + h,   ch.TextCoordsT.GetX(), ch.TextCoordsS.GetY() }
            };

            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * i._scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    s_pShader->StopUsing();
    // clear gl errors
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
}
