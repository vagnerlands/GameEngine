#include "TextArea2DOGL.h"
#include "OglHelper.h"
#include "OGLTypes.h"
#include "IRenderer.h"
#include "glsl.h"
#include "TextRenderer.h"

using namespace Types;
const std::string Graphics::TextArea2DOGL::s_shaderName = "window"; // default shader for a window, not thinking about specializations for now
cwc::glShader* Graphics::TextArea2DOGL::s_pShader = nullptr; // 1 shader to rule them all...

Graphics::TextArea2DOGL::TextArea2DOGL(const IvVector4& background_color, const IvVector3& foreground_color, Float scale) :
    TextArea2D(background_color, foreground_color, scale)
{}

Graphics::TextArea2DOGL::~TextArea2DOGL()
{
    glDeleteVertexArrays(1, &m_data.m_vertexArrayObject);
    glDeleteBuffers(1, &VBO);
}

void Graphics::TextArea2DOGL::Setup()
{
    // clear gl errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
    // generate the shader only once for all windows
    if (s_pShader == nullptr)
    {
        s_pShader = Graphics::OglHelper::generateShader(s_shaderName);
    }

    // same shader for all windows
    m_data.m_pShader = s_pShader;

    Vertices = {
        //                      Vertices                    TextCoords
        SScreenVertex(glm::vec3{0.0f, 1.0f, 0.f}, glm::vec2{0.0f, 1.0f}),
        SScreenVertex(glm::vec3{1.0f, 0.0f, 0.f}, glm::vec2{1.0f, 0.0f}),
        SScreenVertex(glm::vec3{0.0f, 0.0f, 0.f}, glm::vec2{0.0f, 0.0f}),
        SScreenVertex(glm::vec3{0.0f, 1.0f, 0.f}, glm::vec2{0.0f, 1.0f}),
        SScreenVertex(glm::vec3{1.0f, 1.0f, 0.f}, glm::vec2{1.0f, 1.0f}),
        SScreenVertex(glm::vec3{1.0f, 0.0f, 0.f}, glm::vec2{1.0f, 0.0f})
    };
    m_data.m_indicesCount = Vertices.size();
    // create buffers/arrays
    glGenVertexArrays(1, &m_data.m_vertexArrayObject);
    glGenBuffers(1, &VBO);
    
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(SScreenVertex), &Vertices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glBindVertexArray(m_data.m_vertexArrayObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SScreenVertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SScreenVertex), (void*)offsetof(SScreenVertex, TexCoords));
    glBindVertexArray(0);

    // check for GL errors
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
}
void Graphics::TextArea2DOGL::draw(Float dt)
{
    s_pShader->Use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // model with all the transformations
    updateModel();
    s_pShader->setUniformMatrix4fv("model", 1, false, (GLfloat*)m_transforms.GetFloatPtr());
    // projection
    IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
    s_pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
    // color
    s_pShader->setUniform4f("color", m_backgroundColor.GetX(), m_backgroundColor.GetY(), m_backgroundColor.GetZ(), m_backgroundColor.GetW());
    // no texture, always
    s_pShader->setUniform1i("has_texture", (GLint)(0)); 

    // draw all meshes mesh
    glBindVertexArray(m_data.m_vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, m_data.m_indicesCount);
    
    // release all
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_BLEND);
    s_pShader->StopUsing();

    // check for GL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << error << std::endl;
    }
}


