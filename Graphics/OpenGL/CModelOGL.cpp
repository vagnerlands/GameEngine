#include "OpenGL/CModelOGL.h"
#include <iostream>
#include "Model.h"
#include "CTextureHolder.h"
#include "Ilumination.h"
#include "IvMatrix44.h"
#include "SceneItem.h"
#include "MutexFactory.h"
#include "OglHelper.h"

Graphics::CModelOGL::CModelOGL(string modelName) :
	m_vboBufferCreated(false), 
	m_hasAnimations(false), 
	m_isWireMode(false), 
	m_currentBuffer(eBuffering_First),
	m_pBoneMutex(MutexFactory::Instance().Create("Bones")),
	m_pModelImporter()
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

		// destroy shared_ptr
		m_pModelImporter = nullptr;
	}
}



bool Graphics::CModelOGL::Create()
{
	//Model modelLoader(bytesStream, length);
	m_pModelImporter = make_shared<Model>(m_Importer);
	m_pModelImporter->SetBoneInformation(&m_boneInformation);
	// actually tries to load the model
	m_pModelImporter->Load("./Assets/" + m_modelName);

    const aiScene* pScene = m_Importer.GetScene();
    if (pScene != nullptr)
    {
        m_global_inverse_transform = pScene->mRootNode->mTransformation;
    }
	m_global_inverse_transform.Inverse();

	if ((pScene != nullptr) 
        && (pScene->mAnimations != nullptr))
	{
		ticks_per_second = pScene->mAnimations[0]->mTicksPerSecond;
		m_hasAnimations = true;
	}
	else
	{
		ticks_per_second = 25.0f;
	}

	// TODO: check for fail condition
	return true;
}


bool Graphics::CModelOGL::Apply(const Model* pModelInfo)
{
	const Model* pModel = pModelInfo;
	if (pModel == nullptr)
	{
		pModel = m_pModelImporter.get();
	}
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
		m_drawAttr.reserve((*pModel).meshes.size());
		if (m_hasAnimations)
		{
			m_bonesBufferObject.reserve((*pModel).meshes.size());
		}
		m_vertexBufferObject.reserve((*pModel).meshes.size());
		m_elementBufferObject.reserve((*pModel).meshes.size());
		// iterates on each mesh, creating VAO, VBO and Textures
		for (UInt32 i = 0; i < (*pModel).meshes.size(); ++i)
		{
			// first, try to compile the shader
			cwc::glShader* pShader = Graphics::OglHelper::generateShader((*pModel).meshes[i].m_shaderName);

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
				glBufferData(GL_ARRAY_BUFFER, (*pModel).meshes[i].bones_id_weights_for_each_vertex.size() * sizeof((*pModel).meshes[i].bones_id_weights_for_each_vertex[0]), &(*pModel).meshes[i].bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);
 				glBindBuffer(GL_ARRAY_BUFFER, 0);
				m_bonesBufferObject.push_back(VBO_bones);
			}

			m_vertexBufferObject.push_back(VBO);
			m_elementBufferObject.push_back(EBO);
			// copy the texture vector into m_drawAttr.m_textures
			m_drawAttr.push_back(SDrawData(VAO, (*pModel).meshes[i].m_indices.size(), (*pModel).meshes[i].m_textures, pShader));

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, (*pModel).meshes[i].m_vertices.size() * sizeof(SModelVertex), &(*pModel).meshes[i].m_vertices[0], GL_STATIC_DRAW);

			// element buffer object allocation
			if ((*pModel).meshes[i].m_indices.size() > 0)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*pModel).meshes[i].m_indices.size() * sizeof(UInt32), &(*pModel).meshes[i].m_indices[0], GL_STATIC_DRAW);
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

        m_boundaryBox = pModel->m_boundaryBox; // copies the content to local boundary box

		m_vboBufferCreated = true; // mark this as "created" already, to avoid creating the same object twice...

	}
	// destroy shared_ptr
	m_pModelImporter = nullptr;

	return m_vboBufferCreated;
}

void Graphics::CModelOGL::Update(float dt)
{
	// if this has 
	if (m_hasAnimations)
	{		
        if (m_boneTransforms[m_currentBuffer].size() < m_boneInformation.m_num_bones)
        {
            m_boneTransforms[m_currentBuffer].resize(m_boneInformation.m_num_bones);
        }
		boneTransform((double)dt, m_boneTransforms[m_currentBuffer]);
        // we always draw the alternative buffer, next round we'll prepare the next animation on "currentBuffer"
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

	// indexer
	for (UInt32 i = 0; i < m_drawAttr.size(); ++i)
	{
		// when rendering shadows, we must not enable other shaders or activate textures
		if (!isRenderingShadows)
		{
			si.SetUpScene(m_drawAttr[i].m_pShader);
			si.SetUpAnimation(m_drawAttr[i].m_pShader);

			m_pBoneMutex->mutexLock();
			eBuffering buffer = m_currentBuffer == eBuffering_First ? eBuffering_Second : eBuffering_First;
			for (UInt32 s = 0; s < m_boneTransforms[buffer].size(); s++) // move all matrices for actual model position to shader
			{
				m_drawAttr[i].m_pShader->setUniformMatrix4fv(nullptr, 1, GL_TRUE, (GLfloat*)&m_boneTransforms[buffer][s], m_bone_location[s]);
			}
			m_pBoneMutex->mutexUnlock();

			Graphics::OglHelper::applyTextures(m_drawAttr[i].m_pShader, m_drawAttr[i].m_textures);
        }	
		else
		{
			m_pBoneMutex->mutexLock();
			eBuffering buffer = m_currentBuffer == eBuffering_First ? eBuffering_Second : eBuffering_First;
			for (UInt32 s = 0; s < m_boneTransforms[buffer].size(); s++) // move all matrices for actual model position to shader
			{
				Graphics::Ilumination::Instance().UpdateBoneTransformations((Float*)&m_boneTransforms[buffer][s], s);
			}
			m_pBoneMutex->mutexUnlock();

			// setup certain
			si.ShadowsPass();
		}

		if (m_isWireMode)
		{ 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2);
		}

		// draw all meshes mesh
		glBindVertexArray(m_drawAttr[i].m_vertexArrayObject);
		glDrawElements(convertRenderingPrimitive(si.GetRenderingPrimitive()), 
            m_drawAttr[i].m_indicesCount, 
            GL_UNSIGNED_INT, 
            0);

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
	bool retVal = Apply(&*m_pData);

	// release this data (let the shared_ptr destructor take care)
	m_pData = nullptr;

	// return creation status
	return retVal;
}

GLenum Graphics::CModelOGL::convertRenderingPrimitive(eRenderingPrimitive primitive) const
{
    switch (primitive)
    {
        case eRenderingPrimitive::eRenderingPrimitive_TriangleFan:
            return GL_TRIANGLE_FAN;
        case eRenderingPrimitive::eRenderingPrimitive_TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case eRenderingPrimitive::eRenderingPrimitive_Lines:
            return GL_LINES;
        case eRenderingPrimitive::eRenderingPrimitive_Triangles:
        default:        
            return GL_TRIANGLES;
    }
}

UInt32 Graphics::CModelOGL::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (UInt32 i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) 
		{
			return i; 
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
	if (p_node_anim->mNumRotationKeys == 1)
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	UInt32 rotation_index = findRotation(p_animation_time, p_node_anim); 
	UInt32 next_rotation_index = rotation_index + 1; 
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);

	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

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
	if (p_node_anim->mNumScalingKeys == 1) 
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	UInt32 scaling_index = findScaling(p_animation_time, p_node_anim); 
	UInt32 next_scaling_index = scaling_index + 1; 
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);

	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
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
		const aiNodeAnim* node_anim = p_animation->mChannels[i]; 
		if (string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}

	return nullptr;
}
// start from RootNode
void Graphics::CModelOGL::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{

	string node_name(p_node->mName.data);

	const aiAnimation* animation = m_Importer.GetScene()->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name); 

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
	float animation_time = fmod(time_in_ticks, m_Importer.GetScene()->mAnimations[0]->mDuration); 

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

const IvAABB* Graphics::CModelOGL::GetBoundaryBox() const
{
    return &m_boundaryBox;
}
void Graphics::CModelOGL::SetWireMode(bool display)
{
    m_isWireMode = display;
}