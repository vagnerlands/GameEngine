#include "CModelOGL.h"
#include <iostream>
#include "Model.h"
#include "CTextureHolder.h"


Graphics::CModelOGL::CModelOGL() : 
	m_vboBufferCreated(false)
{
    m_elementBufferObject.clear();
    m_drawAttr.clear();
    m_vertexBufferObject.clear();
}

Graphics::CModelOGL::~CModelOGL()
{
	if (m_vboBufferCreated)
	{
        for (UInt32 i = 0; i < m_drawAttr.size(); ++i)
        {
		    // delete VAO
		    glDeleteVertexArrays(1, &m_drawAttr[i].m_vertexArrayObject);
        }

        for (UInt32 i = 0; i < m_elementBufferObject.size(); ++i)
        {
            // delete EBO
            glDeleteBuffers(1, &m_elementBufferObject[i]);
        }

        for (UInt32 i = 0; i < m_vertexBufferObject.size(); ++i)
        {
            // delete VBO
            glDeleteBuffers(1, &m_vertexBufferObject[i]);
        }

        m_vertexBufferObject.clear();
        m_elementBufferObject.clear();
        m_drawAttr.clear();

		// unmark it as "created", cause now it no longer exists
		m_vboBufferCreated = false;
	}
}



bool Graphics::CModelOGL::Create(const Model& modelInfo)
{
    if (!m_vboBufferCreated)
    {
        m_vboBufferCreated = true;

        // clear gl errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Error: " << error << std::endl;
        }

        m_vertexBufferObject.reserve(modelInfo.meshes.size());
        m_drawAttr.reserve(modelInfo.meshes.size());
        m_elementBufferObject.reserve(modelInfo.meshes.size());

        for (UInt32 i = 0; i < modelInfo.meshes.size(); ++i)
        {
            UInt32 VAO, VBO, EBO = 0;
            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            m_vertexBufferObject.push_back(VBO);
            m_elementBufferObject.push_back(EBO);
            m_drawAttr.push_back(SDrawData(VAO, modelInfo.meshes[i].m_indices.size(), modelInfo.meshes[i].m_textures));

            glBindVertexArray(VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, modelInfo.meshes[i].m_vertices.size() * sizeof(SModelVertex), &modelInfo.meshes[i].m_vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelInfo.meshes[i].m_indices.size() * sizeof(UInt32), &modelInfo.meshes[i].m_indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SModelVertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SModelVertex), (void*)offsetof(SModelVertex, Normal));
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SModelVertex), (void*)offsetof(SModelVertex, TexCoords));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SModelVertex), (void*)offsetof(SModelVertex, Tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SModelVertex), (void*)offsetof(SModelVertex, Bitangent));

            glBindVertexArray(0);

            // check for GL errors
            error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << "OpenGL Error: " << error << std::endl;
            }
        }

        m_vboBufferCreated = true;

    }

    return m_vboBufferCreated;
}


void Graphics::CModelOGL::Draw(cwc::glShader* shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (UInt32 i = 0 ; i < m_drawAttr.size(); ++i)
    {
		for (unsigned int ti = 0; ti < m_drawAttr[i].m_textures.size(); ti++)
		{
			glActiveTexture(GL_TEXTURE0 + ti); // active proper texture unit before binding
											  // retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = m_drawAttr[i].m_textures[ti].m_uniformName;
			// now set the sampler to the correct texture unit
			shader->setTexture((char*)name.data(), CTextureHolder::s_pInstance->getTextureById(m_drawAttr[i].m_textures[ti].m_filename));
		}
        // draw all meshes mesh
        glBindVertexArray(m_drawAttr[i].m_vertexArrayObject);
        glDrawElements(GL_TRIANGLES, m_drawAttr[i].m_indicesCount, GL_UNSIGNED_INT, 0);
    }

    // always good practice to set everything back to defaults once configured.
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

}

shared_ptr<Model> Graphics::CModelOGL::Allocate()
{
	// creates a new buffer for the model object
	m_pData = shared_ptr<Model>(new Model());
	// return this buffer for edit
	return m_pData;
}

bool Graphics::CModelOGL::Commit()
{
	// create in the regular way - no duplicated code here
	bool retVal = Create(*m_pData);

	// release this data (let the shared_ptr destructor take care)
    m_pData = nullptr;

	// return creation status
	return retVal;
}
