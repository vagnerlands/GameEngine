#ifndef _OGLTYPES_H_
#define _OGLTYPES_H_

#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include "CommonTypes.h"

#include "IvVector2.h"
#include "IvVector3.h"
#include "IvVector4.h"

#include "GL/glew.h"
#include "GL/glut.h"

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


	struct SModelData
	{
		virtual ~SModelData()
		{

		}

		SModelData() 
		{
			
		}


		vector<UInt16> m_indexes;
		vector<IvVector2> m_textures;
		vector<IvVector3> m_normals;
		vector<IvVector3> m_vertices;
		vector<SMaterialAttr> m_material;
		vector<SFaceAttr> m_faces;
	};

}

#endif //_OGLTYPES_H_
