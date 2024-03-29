#ifndef _Window2DOGL_H_
#define _Window2DOGL_H_

#include "Window2D.h"
#include "OGLTypes.h"

using namespace Types;

namespace cwc
{
    class glShader;
}

namespace Graphics
{
	class Window2DOGL : public Window2D
	{
        static const std::string    s_shaderName;
        static cwc::glShader*       s_pShader;

        SDrawData m_data;
        vector<SScreenVertex> Vertices;
        vector<SModelTexture> Textures;
        UInt32 VBO = 0;
        virtual void draw(Float dt) override;
	public:
        explicit Window2DOGL(const IvVector4& color, const std::string& textureFile);
        virtual ~Window2DOGL();
        virtual void Setup() override; 
	};

}
#endif // _Window2DOGL_H_