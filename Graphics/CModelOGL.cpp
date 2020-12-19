#include "CModelOGL.h"
#include <iostream>
#include "Model.h"
#include "CTextureHolder.h"
#include "IRenderer.h"
#include "Ilumination.h"
#include "IvMatrix44.h"
#include "SceneItem.h"
#include "MutexFactory.h"
#include <time.h> // put this somewhere else, create a lib for time
#include <WinBase.h> // put this somewhere else


Graphics::CModelOGL::CModelOGL(string modelName) :
	m_vboBufferCreated(false), m_hasAnimations(false), m_isWireMode(false), m_currentBuffer(eBuffering_First), m_pBoneMutex(MutexFactory::Instance().Create("Bones"))
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

		if (m_hasAnimations)
		{
			for (UInt32 i = 0; i < m_bonesBufferObject.size(); ++i)
			{
				// delete VBO
				glDeleteBuffers(1, &m_bonesBufferObject[i]);
			}
		}

		m_bonesBufferObject.clear();
		m_vertexBufferObject.clear();
		m_elementBufferObject.clear();
		m_drawAttr.clear();

		// release last loaded scene
		m_Importer.FreeScene();

		// unmark it as "created", cause now it no longer exists
		m_vboBufferCreated = false;
	}
}



bool Graphics::CModelOGL::Create()
{
	//Model modelLoader(bytesStream, length);
	Model modelImporter(m_Importer);
	modelImporter.SetBoneInformation(&m_boneInformation);
	// actually tries to load the model
	modelImporter.Load("./Assets/" + m_modelName);

	m_global_inverse_transform = m_Importer.GetScene()->mRootNode->mTransformation;
	m_global_inverse_transform.Inverse();
	const aiScene* scene = m_Importer.GetScene();

	if (scene->mAnimations != nullptr)
	{
		ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
		m_hasAnimations = true;
	}
	else
	{
		ticks_per_second = 25.0f;
	}

	return Apply(modelImporter);
	
}


bool Graphics::CModelOGL::Apply(const Model& modelInfo)
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
		if (m_hasAnimations)
		{
			m_bonesBufferObject.reserve(modelInfo.meshes.size());
		}
		m_vertexBufferObject.reserve(modelInfo.meshes.size());
		m_elementBufferObject.reserve(modelInfo.meshes.size());
		// iterates on each mesh, creating VAO, VBO and Textures
		for (UInt32 i = 0; i < modelInfo.meshes.size(); ++i)
		{
			// first, try to compile the shader
			cwc::glShader* pShader = generateShader(modelInfo.meshes[i].m_shaderName);

			UInt32 VAO, VBO, EBO, VBO_bones = 0;
			// create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			// bones data
			if (m_hasAnimations)
			{
				// prepare the animated bones				
				for (UInt32 i = 0; i < MAX_BONES; i++) // get location all matrices of bones
				{
					string name = "bones[" + to_string(i) + "]";// name like in shader
					m_bone_location[i] = pShader->GetUniformLocation(name.c_str());
				}
				glGenBuffers(1, &VBO_bones);
				glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
				glBufferData(GL_ARRAY_BUFFER, modelInfo.meshes[i].bones_id_weights_for_each_vertex.size() * sizeof(modelInfo.meshes[i].bones_id_weights_for_each_vertex[0]), &modelInfo.meshes[i].bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);
 				glBindBuffer(GL_ARRAY_BUFFER, 0);
				m_bonesBufferObject.push_back(VBO_bones);
			}

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

			//bones
			if (m_hasAnimations)
			{
				glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
				glEnableVertexAttribArray(5);
				glVertexAttribIPointer(5, 4, GL_INT, sizeof(SVertexBoneData), (GLvoid*)0); // for INT Ipointer
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SVertexBoneData), (GLvoid*)offsetof(SVertexBoneData, weights));
				glBindBuffer(GL_ARRAY_BUFFER, 0);				
			}

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

void Graphics::CModelOGL::Update(float dt)
{
	// if this has 
	if (m_hasAnimations)
	{		
		m_boneTransforms[m_currentBuffer].resize(m_boneInformation.m_num_bones);
		boneTransform((double)dt, m_boneTransforms[m_currentBuffer]);
		m_pBoneMutex->mutexLock();
		m_currentBuffer = (m_currentBuffer == eBuffering_First) ? eBuffering_Second : eBuffering_First;
		m_pBoneMutex->mutexUnlock();
	}
}

void Graphics::CModelOGL::Draw(const SceneItem& si, float dt, bool isRenderingShadows)
{
	// nothing to do here in case we're currently preparing the shadow map
	// and this object doesn't block light
	if ((isRenderingShadows) && (!si.HasShadows()))
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
	rotateModel.Rotation(si.GetRotation());
	// scale transformation
	scaleModel.Scaling(si.GetScale());
	// translation transformation
	translateModel.Translation(si.GetLocation());
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
			if (si.HasShadows())
			{
				m_drawAttr[i].m_pShader->setUniform3f("viewPos",
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetX(),
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetY(),
					Graphics::IRenderer::mRenderer->GetCamera().m_position.GetZ());
				m_drawAttr[i].m_pShader->setUniform3f("lightPos", lightLocation[0], lightLocation[1], lightLocation[2]);
				const IvVector3& lc = Graphics::Ilumination::Instance().GetLightColor();
				m_drawAttr[i].m_pShader->setUniform3f("light_color", lc.GetX(), lc.GetY(), lc.GetZ());
				// update the boolean flag for "has shadows"
				m_drawAttr[i].m_pShader->setUniform1i("cast_shadows", si.HasShadows());
				m_drawAttr[i].m_pShader->setUniform1f("light_attenuation", Graphics::Ilumination::Instance().GetLightAttenuation());
				m_drawAttr[i].m_pShader->setUniform1f("far_plane", Graphics::IRenderer::mRenderer->GetFar());
				m_drawAttr[i].m_pShader->setUniform1i("depthMap", 2);
			}
        }		

		if (m_hasAnimations)
		{
			m_pBoneMutex->mutexLock();
			eBuffering buffer = m_currentBuffer == eBuffering_First ? eBuffering_Second : eBuffering_First;
			for (UInt32 s = 0; s < m_boneTransforms[buffer].size(); s++) // move all matrices for actual model position to shader
			{
				if (isRenderingShadows)
				{
					Graphics::Ilumination::Instance().UpdateBoneTransformations((Float*)&m_boneTransforms[buffer][s], s);
				}
				else
				{
					m_drawAttr[i].m_pShader->setUniformMatrix4fv(nullptr, 1, GL_TRUE, (GLfloat*)&m_boneTransforms[buffer][s], m_bone_location[s]);
				}
			}
			m_pBoneMutex->mutexUnlock();
		}

		if (isRenderingShadows)
		{
			Graphics::Ilumination::Instance().HasAnimations(m_hasAnimations);
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

		if (m_isWireMode)
		{ 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2);
		}

		// draw all meshes mesh
		glBindVertexArray(m_drawAttr[i].m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, m_drawAttr[i].m_indicesCount, GL_UNSIGNED_INT, 0);

		// set back the opengl directives back to what it was
		if (m_isWireMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// always good practice to set everything back to defaults once configured.
		glBindVertexArray(0);
		// when rendering shadows, we must not enable other shaders or activate textures
		if (!isRenderingShadows)
		{
			glActiveTexture(GL_TEXTURE0);
			glUseProgram(0);
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
	bool retVal = Apply(*m_pData);

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
	bool isGeometryFileNotFound = (ret1 == INVALID_FILE_ATTRIBUTES) && (ret2 == ERROR_FILE_NOT_FOUND);

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



UInt32 Graphics::CModelOGL::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
	for (UInt32 i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) // �������� ����� ��������
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
		{
			return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
		}
	}

	assert(0);
	return 0;
}

UInt32 Graphics::CModelOGL::findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (UInt32 i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) 
		{
			return i; 
		}
	}

	assert(0);
	return 0;
}

UInt32 Graphics::CModelOGL::findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (UInt32 i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) 
		{
			return i; 
		}
	}

	assert(0);
	return 0;
}

aiVector3D Graphics::CModelOGL::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1)
	{
		return p_node_anim->mPositionKeys[0].mValue;
	}

	UInt32 position_index = findPosition(p_animation_time, p_node_anim); 
	UInt32 next_position_index = position_index + 1; 
	assert(next_position_index < p_node_anim->mNumPositionKeys);
	
	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
	
	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	//assert(factor >= 0.0f && factor <= 1.0f);
	if (!(factor >= 0.0f && factor <= 1.0f))
	{
		factor = 0.01f;
	}
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Graphics::CModelOGL::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1) // Keys ��� ������� �����
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	UInt32 rotation_index = findRotation(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
	UInt32 next_rotation_index = rotation_index + 1; // ������ ��������� �������� �����
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);
	// ���� ����� �������
	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
	// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	//assert(factor >= 0.0f && factor <= 1.0f);
	if (!(factor >= 0.0f && factor <= 1.0f))
	{
		factor = 0.01f;
	}
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D Graphics::CModelOGL::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1) // Keys ��� ������� �����
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	UInt32 scaling_index = findScaling(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
	UInt32 next_scaling_index = scaling_index + 1; // ������ ��������� �������� �����
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);
	// ���� ����� �������
	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
	// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	//assert(factor >= 0.0f && factor <= 1.0f);
	if (!(factor >= 0.0f && factor <= 1.0f))
	{
		factor = 0.01f;
	}
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim* Graphics::CModelOGL::findNodeAnim(const aiAnimation* p_animation, const string p_node_name)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	for (UInt32 i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i]; // ��������� ������� ������ node
		if (string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;// ���� ����� �������� �� ������� ����� (� ������� ����������� node) ������������ ���� node_anim
		}
	}

	return nullptr;
}
// start from RootNode
void Graphics::CModelOGL::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{

	string node_name(p_node->mName.data);

	//������� node, �� ������� ������������ �������, ������������� �������� ���� ������(aiNodeAnim).
	const aiAnimation* animation = m_Importer.GetScene()->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name); // ����� ������� �� ����� ����

	if (node_anim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		node_transform = translate_matr * rotate_matr * scaling_matr;
	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;

	// ���� � node �� �������� ����������� bone, �� �� node ������ ��������� � ������ bone !!!
	if (m_boneInformation.m_bone_mapping.find(node_name) != m_boneInformation.m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		UInt32 bone_index = m_boneInformation.m_bone_mapping[node_name];
		m_boneInformation.m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_boneInformation.m_bone_matrices[bone_index].offset_matrix;
	}

	for (UInt32 i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}

}

void Graphics::CModelOGL::boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; // = mat4(1.0f);

	double time_in_ticks = time_in_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, m_Importer.GetScene()->mAnimations[0]->mDuration); //������� �� ����� (������� �� ������)
	// animation_time - ���� ������� ������ � ���� ������ �� ������ �������� (�� ������� �������� ����� � �������� )

	readNodeHierarchy(animation_time, m_Importer.GetScene()->mRootNode, identity_matrix);

	for (UInt32 i = 0; i < m_boneInformation.m_num_bones; i++)
	{
		transforms[i] = m_boneInformation.m_bone_matrices[i].final_world_transform;
	}
}

glm::mat4 Graphics::CModelOGL::aiToGlm(aiMatrix4x4 ai_matr)
{
	glm::mat4 result;
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;

	//cout << " " << result[0].x << "		 " << result[0].y << "		 " << result[0].z << "		 " << result[0].w << endl;
	//cout << " " << result[1].x << "		 " << result[1].y << "		 " << result[1].z << "		 " << result[1].w << endl;
	//cout << " " << result[2].x << "		 " << result[2].y << "		 " << result[2].z << "		 " << result[2].w << endl;
	//cout << " " << result[3].x << "		 " << result[3].y << "		 " << result[3].z << "		 " << result[3].w << endl;
	//cout << endl;

	//cout << " " << ai_matr.a1 << "		 " << ai_matr.b1 << "		 " << ai_matr.c1 << "		 " << ai_matr.d1 << endl;
	//cout << " " << ai_matr.a2 << "		 " << ai_matr.b2 << "		 " << ai_matr.c2 << "		 " << ai_matr.d2 << endl;
	//cout << " " << ai_matr.a3 << "		 " << ai_matr.b3 << "		 " << ai_matr.c3 << "		 " << ai_matr.d3 << endl;
	//cout << " " << ai_matr.a4 << "		 " << ai_matr.b4 << "		 " << ai_matr.c4 << "		 " << ai_matr.d4 << endl;
	//cout << endl;

	return result;
}

aiQuaternion Graphics::CModelOGL::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	//cout << a.w + a.x + a.y + a.z << endl;
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}