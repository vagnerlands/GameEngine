#include "CShaderHolder.h"
#include "gl/glut.h"

#include <time.h>
#include <iostream>
#include <string>
#ifdef WIN32
#include "CWinMutex.h"
#endif

CShaderHolder* CShaderHolder::s_pInstance = NULL;

CShaderHolder*
CShaderHolder::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CShaderHolder();
	}

	return s_pInstance;
}

void 
CShaderHolder::OnRemoveEvent(string removeItem)
{
	instance()->RemoveShader(removeItem);
}

CShaderHolder::CShaderHolder()
{
#ifdef WIN32
	m_pShaderContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
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
	char vertexFilename[200], fragmentFilename[200];
	sprintf(vertexFilename, "./%sVertexshader.txt", shaderId.data());
	sprintf(fragmentFilename, "./%sFragmentshader.txt", shaderId.data());

	obj = m_shaderManager.loadfromFile(vertexFilename, fragmentFilename);

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
CShaderHolder::getShaderById(string shaderId)
{
	// then try to find it in the textures map
	ShadersMap::iterator result = m_shaders.find(shaderId);
	if (result != m_shaders.end())
	{
		return result->second;
	}

	// cache miss - then add this texture to the process list
	LoadShader(shaderId);
	
	// if it somehow failed, returns -1
	return false;
}


CShaderHolder::~CShaderHolder()
{
	while (!m_shaders.empty())
	{
		m_shaders.erase(m_shaders.begin());
	}		
}