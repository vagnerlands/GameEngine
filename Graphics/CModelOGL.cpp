#include "CModelOGL.h"
#include <iostream>
#include "Model.h"
#include "CTextureHolder.h"
#include "IRenderer.h"
#include "Ilumination.h"
#include "IvMatrix44.h"
#include <time.h> // put this somewhere else, create a lib for time
#include <WinBase.h> // put this somewhere else


Graphics::CModelOGL::CModelOGL(string modelName) :
	m_vboBufferCreated(false)
{
	// only for debug purposes
	m_modelName = modelName;
	m_elementBufferObject.clear();
	m_drawAttr.clear();
	m_vertexBufferObject.clear();
	// safe initialization of rotation
	m_rotation.Identity();
}

Graphics::CModelOGL::~CModelOGL()
{
	if (m_vboBufferCreated)
	{
		for (UInt32 i = 0; i < m_drawAttr.size(); ++i)
		{
			// delete VAO
			glDeleteVertexArrays(1, &m_drawAttr[i].m_vertexArrayObject);


			if (m_drawAttr[i].m_pShader != nullptr)
			{
				// destructor releases resources
				delete m_drawAttr[i].m_pShader;
				m_drawAttr[i].m_pShader = nullptr;
			}
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

		// improves performance by avoiding resizing the vector during the operation
		m_drawAttr.reserve(modelInfo.meshes.size());
		m_vertexBufferObject.reserve(modelInfo.meshes.size());
		m_elementBufferObject.reserve(modelInfo.meshes.size());
		// iterates on each mesh, creating VAO, VBO and Textures
		for (UInt32 i = 0; i < modelInfo.meshes.size(); ++i)
		{
			// first, try to compile the shader
 			cwc::glShader* pShader = generateShader(modelInfo.meshes[i].m_shaderName);

			UInt32 VAO, VBO, EBO = 0;
			// create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			m_vertexBufferObject.push_back(VBO);
			m_elementBufferObject.push_back(EBO);
			// copy the texture vector into m_drawAttr.m_textures
			m_drawAttr.push_back(SDrawData(VAO, modelInfo.meshes[i].m_indices.size(), modelInfo.meshes[i].m_textures, pShader));

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, modelInfo.meshes[i].m_vertices.size() * sizeof(SModelVertex), &modelInfo.meshes[i].m_vertices[0], GL_STATIC_DRAW);

			// element buffer object allocation
			if (modelInfo.meshes[i].m_indices.size() > 0)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelInfo.meshes[i].m_indices.size() * sizeof(UInt32), &modelInfo.meshes[i].m_indices[0], GL_STATIC_DRAW);
			}

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

void Graphics::CModelOGL::Draw(bool isRenderingShadows)
{
	// nothing to do here in case we're currently preparing the shadow map
	// and this object doesn't block light
	if ((isRenderingShadows) && (!m_hasShadow))
	{
		return;
	}

	// clear any non-treated Gl errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Untreated error has been found");

	// identity
	// final model for the shader - each transformation should be calculated
	// alone and then combined
	IvMatrix44 model;
	// translate model
	IvMatrix44 translateModel;
	translateModel.Identity();
	// rotate model
	IvMatrix44 rotateModel;
	rotateModel.Identity();
	// scale model
	IvMatrix44 scaleModel;
	scaleModel.Identity();
	// calculates idependently each transformation
	// rotate transformation
	rotateModel.Rotation(m_rotation);
	// scale transformation
	scaleModel.Scaling(m_scale);
	// translation transformation
	translateModel.Translation(m_location);
	// combine both transformations
	model = translateModel * scaleModel * rotateModel;

	// indexer
	for (UInt32 i = 0; i < m_drawAttr.size(); ++i)
	{
		// when rendering shadows, we must not enable other shaders or activate textures
		if (!isRenderingShadows)
		{
			// get shader program id 
			GLuint programId = m_drawAttr[i].m_pShader->GetProgramObject();
			// enable it
			glUseProgram(programId);
			// check for errors
			Int32 glErr = glGetError();
			if (glErr != 0)
				DEBUG_OUT("Failed to apply shader ");

			// Apply attributes known for this shader
			IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
			IvMatrix44& viewMatrix = Graphics::IRenderer::mRenderer->GetViewMatrix();

			Float lightLocation[4] = { 0 };
			Graphics::Ilumination::Instance().GetIluminationItemLocationPtr("main", lightLocation);

			m_drawAttr[i].m_pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
			m_drawAttr[i].m_pShader->setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());
			if (m_hasShadow)
			{
				m_drawAttr[i].m_pShader->setUniform3f("lightPos", lightLocation[0], lightLocation[1], lightLocation[2]);
			}

			if (m_hasShadow)
			{
				// these parameters are important for the depth shadow map 
				m_drawAttr[i].m_pShader->setUniform3f("viewPos",
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetX(),
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetY(),
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetZ());
				m_drawAttr[i].m_pShader->setUniform1f("far_plane", Graphics::IRenderer::mRenderer->GetFar());
				glErr = glGetError();
				m_drawAttr[i].m_pShader->setUniform1i("depthMap", 2);
				glErr = glGetError();
			}
		}

		glErr = glGetError();
		if (isRenderingShadows)
		{
			Graphics::Ilumination::Instance().UpdateModel(model);
		}
		else
		{
			m_drawAttr[i].m_pShader->setUniformMatrix4fv("model", 1, false, (GLfloat*)model.GetFloatPtr());
		}

		// when rendering shadows, we must not enable other shaders or activate textures
		if (!isRenderingShadows)
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
					m_drawAttr[i].m_pShader->setTexture((char*)name.data(), CTextureHolder::s_pInstance->getTextureVector(m_drawAttr[i].m_textures));
				}
				else
				{
					// texture indexer
					UInt32 ti = 0;
					for (; ti < cTexturesCount; ti++)
					{
						glActiveTexture(GL_TEXTURE0 + ti); // active proper texture unit before binding
														  // retrieve texture number (the N in diffuse_textureN)
						string name = m_drawAttr[i].m_textures[ti].m_uniformName;
						// now set the sampler to the correct texture unit
						m_drawAttr[i].m_pShader->setTexture((char*)name.data(), CTextureHolder::s_pInstance->getTextureById(m_drawAttr[i].m_textures[ti].m_filename));
					}

					// if this object casts shadow, then apply the fbo shadow texture
					//if (m_hasShadow)
					{
						glActiveTexture(GL_TEXTURE0 + ti);
						glBindTexture(GL_TEXTURE_CUBE_MAP, Graphics::Ilumination::Instance().GetShadowTexture());
					}

				}
			}
		}
		// draw all meshes mesh
		glBindVertexArray(m_drawAttr[i].m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, m_drawAttr[i].m_indicesCount, GL_UNSIGNED_INT, 0);

		// always good practice to set everything back to defaults once configured.
		glBindVertexArray(0);
		// when rendering shadows, we must not enable other shaders or activate textures
		if (!isRenderingShadows)
		{
			glActiveTexture(GL_TEXTURE0);
			glUseProgram(0);
			glErr = glGetError();
			if (glErr != 0)
				DEBUG_OUT("Failed halting shader");
		}
	}
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

#pragma optimize( "", off )
cwc::glShader* Graphics::CModelOGL::generateShader(const string& shaderName)
{
	// return value
	bool status = true;
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* pShader = nullptr;
	char vertexFilename[128], fragmentFilename[128], geometryFilename[128];
	sprintf(vertexFilename, "./Assets/%sVertexshader.txt", shaderName.data());
	sprintf(fragmentFilename, "./Assets/%sFragmentshader.txt", shaderName.data());
	sprintf(geometryFilename, "./Assets/%sGeometryshader.txt", shaderName.data());
	// there should be a specialized class for this whole thing
	cwc::glShaderManager shaderLoader;

	DWORD ret1 = GetFileAttributes((LPCWSTR)geometryFilename);
	int ret2 = GetLastError();
	bool isGeometryFileNotFound = (ret1 == INVALID_FILE_ATTRIBUTES) && (ret2== ERROR_FILE_NOT_FOUND);

	// checks if the geometry shader actually is required
	if (isGeometryFileNotFound)
	{
		//File not found
		pShader = shaderLoader.loadfromFile(vertexFilename, fragmentFilename);
	}
	else
	{
		pShader = shaderLoader.loadfromFile(vertexFilename, geometryFilename, fragmentFilename);
	}


	if (pShader == nullptr)
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderName.data());
		status = false;
	}

	// time measurement
	printf(" [*] loading shader [%s] %.2fms\n", shaderName.data(), (float)(clock() - start));

	return pShader;
}
#pragma optimize( "", on )
