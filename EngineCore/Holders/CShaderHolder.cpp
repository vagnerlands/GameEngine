#include "CShaderHolder.h"
#include "MutexFactory.h"
#include "gl/glut.h"

#include <time.h>
#include <iostream>
#include <string>


CShaderHolder* CShaderHolder::s_pInstance = NULL;

bool
CShaderHolder::Create()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CShaderHolder();
		// perform loading of all shaders you want down here

		return true;
	}

	return false;
}

void 
CShaderHolder::OnRemoveEvent(string removeItem)
{
	s_pInstance->RemoveShader(removeItem);
}

CShaderHolder::CShaderHolder()
{
	m_pShaderContentMapMutex = MutexFactory::Instance().Create("ShaderMutex");
	if (m_pShaderContentMapMutex != NULL)
	{
		m_pShaderContentMapMutex->createMutex("ShaderContentMap");
	}
	
}

void 
CShaderHolder::LoadShader(const string shaderId)
{
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* obj = 0;
	char vertexFilename[128], fragmentFilename[128], geometryFilename[128];
	sprintf(vertexFilename, "../Game/Assets/%sVertexshader.txt", shaderId.data());
	sprintf(fragmentFilename, "../Game/Assets/%sFragmentshader.txt", shaderId.data());
	sprintf(geometryFilename, "../Game/Assets/%sGeometryshader.txt", shaderId.data());

	// checks if the geometry shader actually is required
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes((LPCWSTR)geometryFilename) && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		//File not found
		obj = m_shaderManager.loadfromFile(vertexFilename, fragmentFilename);
	}
	else
	{
		obj = m_shaderManager.loadfromFile(vertexFilename, geometryFilename, fragmentFilename);
	}

	if (obj != 0)
	{
		m_shaders.insert(make_pair(shaderId, obj));
	}
	else
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderId);
	}

	// time measurement
	printf(" loading shader [%s] %.2fms\n", shaderId.data(), (float)(clock() - start));
}

void 
CShaderHolder::RemoveShader(const string shaderId)
{
	ShadersMap::iterator it = m_shaders.find(shaderId);
	if (it == m_shaders.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	m_shaders.erase(it);

	//_CrtDumpMemoryLeaks();
}

cwc::glShader* 
CShaderHolder::GetShaderProgramById(const string shaderId)
{
	// then try to find it in the textures map
	ShadersMap::iterator result = m_shaders.find(shaderId);
	if (result != m_shaders.end())
	{
		return result->second;
	}

	return NULL;
}

bool
CShaderHolder::UseShaderById(const string shaderId)
{
	// clear all GL errors
	Int32 glErr = glGetError();

	// then try to find it in the textures map
	ShadersMap::iterator result = m_shaders.find(shaderId);
	if (result != m_shaders.end())
	{
		GLuint programId = result->second->GetProgramObject();
		glUseProgram(programId);
		glErr = glGetError();
		if (glErr != 0)
			return false;
		else
			return true;
	}

	// cache miss - then add this texture to the process list
	LoadShader(shaderId);
	
	// if it somehow failed, returns -1
	return false;
}

bool CShaderHolder::StopShader()
{
	glUseProgram(0);

	Int32 glErr = glGetError();
	if (glErr != 0)
		return false;
	else
		return true;
}


CShaderHolder::~CShaderHolder()
{
	while (!m_shaders.empty())
	{
		m_shaders.erase(m_shaders.begin());
	}		
}