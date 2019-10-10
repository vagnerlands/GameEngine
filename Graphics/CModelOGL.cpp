#include "CModelOGL.h"
#include <iostream>
#include "Model.h"
#include "CTextureHolder.h"
#include "IRenderer.h"
#include "Ilumination.h"
#include "IvMatrix44.h"
#include <time.h>

Graphics::CModelOGL::CModelOGL(string modelName) :
	m_vboBufferCreated(false)
{
	// only for debug purposes
	m_modelName = modelName;
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

		if (m_pShader != nullptr)
		{
			delete m_pShader;
			m_pShader = nullptr;
		}

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

		// improves performance by avoiding resizing the vector during the operation
        m_drawAttr.resize(modelInfo.meshes.size());
		m_vertexBufferObject.resize(modelInfo.meshes.size());
        m_elementBufferObject.resize(modelInfo.meshes.size());
		// iterates on each mesh, creating VAO, VBO and Textures
        for (UInt32 i = 0; i < modelInfo.meshes.size(); ++i)
        {
            UInt32 VAO, VBO, EBO = 0;
            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            m_vertexBufferObject.push_back(VBO);
            m_elementBufferObject.push_back(EBO);
			// copy the texture vector into m_drawAttr.m_textures
            m_drawAttr.push_back(SDrawData(VAO, modelInfo.meshes[i].m_indices.size(), modelInfo.meshes[i].m_textures));

            glBindVertexArray(VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, modelInfo.meshes[i].m_vertices.size() * sizeof(SModelVertex), &modelInfo.meshes[i].m_vertices[0], GL_STATIC_DRAW);

			// element buffer object allocation
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

bool Graphics::CModelOGL::SetShader(const string & shaderName)
{
	// return value
	bool status = true;
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* obj = 0;
	char vertexFilename[200], fragmentFilename[200];
	sprintf(vertexFilename, "../Game/Assets/%sVertexshader.txt", shaderName.data());
	sprintf(fragmentFilename, "../Game/Assets/%sFragmentshader.txt", shaderName.data());
	// there should be a specialized class for this whole thing
	cwc::glShaderManager shaderLoader;
	m_pShader = shaderLoader.loadfromFile(vertexFilename, fragmentFilename);

	if (m_pShader == 0)
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderName);
		status = false;
	}

	// time measurement
	printf(" loading shader [%s] %.2fms\n", shaderName.data(), (float)(clock() - start));

	return status;
}


void Graphics::CModelOGL::Draw()
{
	// get shader program id 
	GLuint programId = m_pShader->GetProgramObject();
	// enable it
	glUseProgram(programId);
	// check for errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Failed to apply shader ");

	// Apply attributes known for this shader
	IvMatrix44 projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
	IvMatrix44 viewMatrix = Graphics::IRenderer::mRenderer->GetViewMatrix();
	// identity
	// final model for the shader - each transformation should be calculated
	// alone and then combined
	IvMatrix44 model;
	// translate model
	IvMatrix44 translateModel;
	translateModel.Identity();
	// scale model
	IvMatrix44 scaleModel;	
	scaleModel.Identity();
	// calculates idependently each transformation

	// scale transformation
	scaleModel.Scaling(m_scale);
	// translation transformation
	translateModel.Translation(m_location);
	// combine both transformations
	model = scaleModel * translateModel;
	Float lightLocation[4] = { 0 };
	Graphics::Ilumination::Instance().GetIluminationItemLocationPtr("main", lightLocation);

	m_pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
	m_pShader->setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());
	m_pShader->setUniformMatrix4fv("model", 1, false, (GLfloat*)model.GetFloatPtr());
	m_pShader->setUniform3f("lightPos", lightLocation[0], lightLocation[1], lightLocation[2]);

	// indexer
    for (UInt32 i = 0 ; i < m_drawAttr.size(); ++i)
    {
		const UInt32 cTexturesCount = m_drawAttr[i].m_textures.size();
		if (cTexturesCount > 0)
		{
			// checks whether is a cubemap - in this case, there will be a texture id for the 6 cube faces
			if (m_drawAttr[i].m_textures[0].m_isCubeMap)
			{
				// who cares, set the first one as the active one
				glActiveTexture(GL_TEXTURE0); 
				string name = m_drawAttr[i].m_textures[0].m_uniformName;
				// now set the sampler to the correct texture unit
				m_pShader->setTexture((char*)name.data(), CTextureHolder::s_pInstance->getTextureVector(m_drawAttr[i].m_textures));
			}
			else
			{
				// texture indexer
				for (UInt32 ti = 0; ti < cTexturesCount; ti++)
				{
					glActiveTexture(GL_TEXTURE0 + ti); // active proper texture unit before binding
													  // retrieve texture number (the N in diffuse_textureN)
					string name = m_drawAttr[i].m_textures[ti].m_uniformName;
					// now set the sampler to the correct texture unit
					m_pShader->setTexture((char*)name.data(), CTextureHolder::s_pInstance->getTextureById(m_drawAttr[i].m_textures[ti].m_filename));
				}
			}
		}
        // draw all meshes mesh
        glBindVertexArray(m_drawAttr[i].m_vertexArrayObject);
        glDrawElements(GL_TRIANGLES, m_drawAttr[i].m_indicesCount, GL_UNSIGNED_INT, 0);
    }

    // always good practice to set everything back to defaults once configured.
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
	glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Failed halting shader");
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
