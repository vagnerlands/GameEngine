#include "CModelOGL.h"
#include <iostream>

Graphics::CModelOGL::CModelOGL() : 
	m_vboBufferCreated(false), 
	m_vertexArrayObject(0),
	m_numberOfIndexes(0)
{
	for (Int32 i = 0; i < VertexBuffer_Max_Num; i++)
	{
		m_elementBuffer[i] = 0;
	}
}

Graphics::CModelOGL::~CModelOGL()
{
	if (m_vboBufferCreated)
	{
		// delete VAO
		glDeleteVertexArrays(1, &m_vertexArrayObject);
		m_vertexArrayObject = 0;

		// must delete each verte
		glDeleteBuffers(VertexBuffer_Max_Num, m_elementBuffer);

		// unmark it as "created", cause now it no longer exists
		m_vboBufferCreated = false;
	}
}

bool Graphics::CModelOGL::Create(const SModelData & modelInfo)
{
	if (!m_vboBufferCreated)
	{
		m_vboBufferCreated = true;

		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error: " << error << std::endl;
		}

		// generates the VAO
		glGenVertexArrays(1, &m_vertexArrayObject);
		// binds the VAO for this model
		glBindVertexArray(m_vertexArrayObject);
		// [          Load buffers          ]
		//vertices
		glGenBuffers(Types::VertexBuffer_Max_Num, m_elementBuffer);
		int oglErr = glGetError();
		if (oglErr != 0)
		{
			printf("glError after glGenBuffers =%d\n", glGenBuffers);
		}

		// [VERTEX PREPARATION]
		glBindBuffer(GL_ARRAY_BUFFER, m_elementBuffer[Types::VertexBuffer_Vertices]);
		glBufferData(GL_ARRAY_BUFFER, (modelInfo.m_vertices.size() * sizeof(IvVector3)), &modelInfo.m_vertices[0], GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 0, (void*)(0));
		// [------------------]

		// [NORMAL PREPARATION]
		glBindBuffer(GL_ARRAY_BUFFER, m_elementBuffer[Types::VertexBuffer_Normals]);
		glBufferData(GL_ARRAY_BUFFER, (modelInfo.m_normals.size() * sizeof(IvVector3)), &modelInfo.m_normals[0], GL_STATIC_DRAW);
		glNormalPointer(GL_FLOAT, 0, (void*)(0));
		// [------------------]

		// [TEXTURE PREPARATION]
		if (modelInfo.m_textures.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_elementBuffer[Types::VertexBuffer_Textures]);
			glBufferData(GL_ARRAY_BUFFER, (modelInfo.m_textures.size() * sizeof(IvVector2)), &modelInfo.m_textures[0], GL_STATIC_DRAW);
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)(0));
		}
		else
		{
			// for AMD Radeon R9 M200X enabling textures directive with a non declared buffer for textures 
			// leads to a fatal error in atioglxx.dll
			// to bypass this error, in case we don't have textures, then we create a dummy map of textures
			// here
			Float textCoords[] = { 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 0.0F, 1.0F };
			glBindBuffer(GL_ARRAY_BUFFER, m_elementBuffer[Types::VertexBuffer_Textures]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), &textCoords, GL_STATIC_DRAW);
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)(0));

		}
		// [------------------]

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer[Types::VertexBuffer_Element]);
		const Int32 numberOfIndexes = modelInfo.m_indexes.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numberOfIndexes, &modelInfo.m_indexes[0], GL_STATIC_DRAW);
		m_numberOfIndexes = numberOfIndexes;

		// allocation integrity check
		Int32 allocIntegrityChk = 0;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &allocIntegrityChk);
		if (allocIntegrityChk != modelInfo.m_indexes.size() * sizeof(GLushort))
		{
			glDeleteVertexArrays(1, &m_vertexArrayObject);
			m_vertexArrayObject = 0;
			glDeleteBuffers(Types::VertexBuffer_Max_Num, m_elementBuffer);
			printf(" <!> error while allocating element buffer \n");
			m_vboBufferCreated = false;
		}
		/*else
		{
			// if the everything was passed to the GPU, no need to keep it in the RAM, so release it...
			modelInfo.m_indexes = vector< UInt16 >();
			modelInfo.m_textures = vector< IvVector2 >();
			modelInfo.m_normals = vector< IvVector3 >();
			modelInfo.m_vertices = vector< IvVector3 >();
			modelInfo.m_material = vector< SMaterialAttr >();
			modelInfo.m_faces = vector< SFaceAttr >();
		}*/

		error = glGetError();

		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error while creating model : " << error << std::endl;
		}
		// [--------------------------------]
	}

	return true;
}

void Graphics::CModelOGL::Draw()
{
	// prepare to render the model
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_numberOfIndexes, GL_UNSIGNED_SHORT, (void*)(0));
	// retrieve any rendering error
	Int32 err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError Drawing Model = %d\n", err);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

SModelData& Graphics::CModelOGL::Allocate()
{
	// creates a new buffer for the model object
	m_pData = new SModelData();
	// return this buffer for edit
	return *m_pData;
}

bool Graphics::CModelOGL::Commit()
{
	// create in the regular way - no duplicated code here
	bool retVal = Create(*m_pData);

	// release this data
	delete m_pData;

	// return creation status
	return retVal;
}
