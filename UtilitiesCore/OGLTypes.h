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

#include <glm/glm.hpp>

#include "GL/glew.h"
#include "GL/glut.h"

#include <Assimp/Importer.hpp>

#define NUM_BONES_PER_VERTEX 4

using namespace std;

namespace Types 
{

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
		SModelTexture() 
			: m_isCubeMap(false)
		{

		}
        // either diffuse, normal or specular
        std::string m_uniformName;
        // name of the texture - may be the full path to the texture, who knows...
        std::string m_filename;
		// should this texture be binded as a CUBE_MAP?
		bool		m_isCubeMap;
    };

    struct SModelVertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };

    // axis aligned bounding box
    struct AABB
    {
        AABB(): Min { 0 }, Max{ 0 } {}
        glm::vec3 Min;
        glm::vec3 Max;
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
        SModelMesh()
        {

        }
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
        vector<SModelVertex>    m_vertices;
        vector<UInt32>          m_indices;
        vector<SModelTexture>   m_textures;
		// bone information
		vector<SVertexBoneData>	bones_id_weights_for_each_vertex;
        // axis aligned bounding box for clipping/collision purposes
        AABB                    m_aabb;
		// shader name (Model.h suggests a name)
		string					m_shaderName = "model";
    };
	
}

#endif //_OGLTYPES_H_
