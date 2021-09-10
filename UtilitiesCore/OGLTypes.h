#ifndef _OGLTYPES_H_
#define _OGLTYPES_H_

#include <memory>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include "CommonTypes.h"

#include "IvVector2.h"
#include "IvVector3.h"
#include "IvVector4.h"

#include "Collision/IvAABB.h"

#include <glm/glm.hpp>

#include "GL/glew.h"
#include "GL/glut.h"

#include <Assimp/Importer.hpp>

#define NUM_BONES_PER_VERTEX 4

namespace cwc
{
    class glShader;
}

using namespace std;

namespace Types 
{

    struct SScreenVertex
    {
        SScreenVertex(const glm::vec3& pos, const glm::vec2& tex) : Position(pos), TexCoords(tex) {}
        // position
        glm::vec3 Position;
        // texCoords
        glm::vec2 TexCoords;
    };

	enum EVertexBuffers
	{
		VertexBuffer_Vertices,
		VertexBuffer_Normals,
		VertexBuffer_Textures,
		VertexBuffer_Colors,
		VertexBuffer_Element,
		VertexBuffer_Max_Num
	};

	enum EDrawDirective
	{
		DrawDirective_Triangles, //GL_TRIANGLES
		DrawDirective_Triangle_Strip, //GL_TRIANGLE_STRIP
		DrawDirective_Triangle_Fan, //GL_TRIANGLE_FAN
		DrawDirective_Max_Num
	};

	struct SMaterialAttr
	{
		IvVector3 m_ambient;
		IvVector3 m_diffuse;
		IvVector3 m_specular;
		Double m_reflect;
		Double m_refract;
		Double m_trans;
		Double m_shiny;
		Double m_glossy;
	};

	struct SFaceAttr
	{
		// vec4 since it supports triangles or quads
		IvVector4 m_vertexIndex;
		IvVector4 m_normalIndex;
		IvVector4 m_textureIndex;
		Int32 m_vertexCount;
		Int32 m_materialIndex;
	};

    struct SModelTexture
    {        
		SModelTexture() : m_isCubeMap(false) {}
        explicit SModelTexture(const char* uniform, const char* file) : m_uniformName(uniform), m_filename(file), m_isCubeMap(false) {}
        // either diffuse, normal or specular
        std::string m_uniformName;
        // name of the texture - may be the full path to the texture, who knows...
        std::string m_filename;
		// should this texture be binded as a CUBE_MAP?
		bool		m_isCubeMap;
    };

    struct SDrawData
    {
        SDrawData() : m_vertexArrayObject(0U), m_indicesCount(0U) {}
        explicit SDrawData(UInt32 vao, UInt32 indCount, vector<SModelTexture> vTextures, cwc::glShader* pShader) :
            m_vertexArrayObject(vao),
            m_indicesCount(indCount),
            m_textures(vTextures),
            m_pShader(pShader)
        {
            // empty
        }

        vector<SModelTexture>   m_textures;
        UInt32					m_vertexArrayObject;
        UInt32					m_indicesCount;

        cwc::glShader*			m_pShader;
    };

    struct SModelVertex
    {
        // position
        IvVector3 Position = { 0.f, 0.f, 0.f };
        // normal
        IvVector3 Normal = { 0.f, 0.f, 0.f};
        // texCoords
        IvVector2 TexCoords = { 0.f, 0.f };
        // tangent
        IvVector3 Tangent = {0.f, 0.f, 0.f};
        // bitangent
        IvVector3 Bitangent = { 0.f, 0.f, 0.f};
    };


	struct SVertexBoneData
	{
		UInt32 ids[NUM_BONES_PER_VERTEX] = {};   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
		float weights[NUM_BONES_PER_VERTEX] = {};

		SVertexBoneData()
		{
			memset(ids, 0, sizeof(ids));    // init all values in array = 0
			memset(weights, 0, sizeof(weights));
		}

		void addBoneData(UInt32 bone_id, float weight) 
		{
			for (UInt32 i = 0; i < NUM_BONES_PER_VERTEX; i++)
			{
				if (weights[i] == 0.0)
				{
					ids[i] = bone_id;
					weights[i] = weight;
					return;
				}
			}
		}
	};

	struct BoneMatrix
	{
		aiMatrix4x4 offset_matrix;
		aiMatrix4x4 final_world_transform;
	};

	struct SBoneInformation
	{
		// bones data
		map<string, UInt32>		m_bone_mapping; // maps a bone name and their index
		UInt32					m_num_bones = 0;
		vector<BoneMatrix>		m_bone_matrices;
		aiMatrix4x4				m_global_inverse_transform;
	};

    struct SModelMesh
    {
        vector<SModelVertex>    m_vertices;
        vector<UInt32>          m_indices;
        vector<SModelTexture>   m_textures;
        // bone information
        vector<SVertexBoneData>	bones_id_weights_for_each_vertex;
        // axis aligned bounding box for clipping/collision purposes
        IvAABB                  m_aabb;
        // shader name (Model.h will try to resolve the shader based on certain )
        string					m_shaderName = "model";

        SModelMesh() {}
        explicit SModelMesh(const SModelMesh& rhs)
        {
            m_vertices = std::move(rhs.m_vertices);
            m_indices = std::move(rhs.m_indices);
            m_textures = std::move(rhs.m_textures);
            bones_id_weights_for_each_vertex = std::move(rhs.bones_id_weights_for_each_vertex);
            m_aabb = rhs.m_aabb;
            m_shaderName = rhs.m_shaderName;
        }
        SModelMesh(SModelMesh&& rhs)
        {
            m_vertices = std::move(rhs.m_vertices);
            m_indices = std::move(rhs.m_indices);
            m_textures = std::move(rhs.m_textures);
            bones_id_weights_for_each_vertex = std::move(rhs.bones_id_weights_for_each_vertex);
            m_aabb = rhs.m_aabb;
            m_shaderName = rhs.m_shaderName;
        }
        SModelMesh& operator=(SModelMesh&& rhs)
        {
            if (&rhs != this)
            {
                m_vertices = std::move(rhs.m_vertices);
                m_indices = std::move(rhs.m_indices);
                m_textures = std::move(rhs.m_textures);
                bones_id_weights_for_each_vertex = std::move(rhs.bones_id_weights_for_each_vertex);
                m_aabb = rhs.m_aabb;
                m_shaderName = rhs.m_shaderName;
            }
            return *this;
        }
		~SModelMesh()
		{
			m_textures = vector<SModelTexture>();
			m_indices = vector<UInt32>();
			m_vertices = vector<SModelVertex>();
			bones_id_weights_for_each_vertex = vector<SVertexBoneData>();
		}
    };
	
}

#endif //_OGLTYPES_H_
