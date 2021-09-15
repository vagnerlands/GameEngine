#ifndef _TextArea2DOGL_H_
#define _TextArea2DOGL_H_

#include "TextArea2D.h"
#include "OGLTypes.h"

using namespace Types;

namespace cwc
{
    class glShader;
}

namespace Graphics
{
	class TextArea2DOGL : public TextArea2D
	{
        static const std::string    s_shaderName;
        static cwc::glShader*       s_pShader;
        
        SDrawData m_data;
        vector<SScreenVertex> Vertices;
        UInt32 VBO = 0;
        virtual void draw(Float dt) override;
	public:
        explicit TextArea2DOGL(const IvVector4& background_color, const IvVector3& foreground_color, Float scale);
        virtual ~TextArea2DOGL();
        virtual void Setup() override; 
	};

}
#endif // _Window2DOGL_H_