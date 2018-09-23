#include "CTextureHolder.h"
#include "gl/glut.h"
#include <time.h>
#ifdef WIN32
#include "CWinMutex.h"
#endif

#include "CFactory2dImage.h"

CTextureHolder* CTextureHolder::s_pInstance = NULL;

bool CTextureHolder::Create(const string pathToTexturesFile)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CTextureHolder(pathToTexturesFile);
		// tries to open the resource file - out of the constructor so errors may be reported
		if ((s_pInstance != nullptr) && (s_pInstance->m_textureFiles->VOpen()))
		{
			return true;
		}
	}
	return false;
}



void 
CTextureHolder::OnRemoveEvent(string removeItem)
{
	s_pInstance->RemoveTexture(removeItem);
}

CTextureHolder::CTextureHolder(const string pathToTexturesFile) :
	m_textureFiles(new CResourceZipFile(pathToTexturesFile.data(), this->OnRemoveEvent))
{
#ifdef WIN32
	m_textureContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
	if (m_textureContentMapMutex != NULL)
	{
		m_textureContentMapMutex->createMutex("TextureContentMap");
	}
}

void 
CTextureHolder::LoadTexture(const string textId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resourceItem(textId);

	Byte* textureDataStream = m_textureFiles->VAllocateAndGetResource(resourceItem);
	// status OK
	if (textureDataStream != 0)
	{
		// checks the first 2 bytes of the stream to know if we know how to parse it
		Int16 fileType(0);
		memcpy(&fileType, textureDataStream, 2);
		I2dImage* pRawImage = CFactory2dImage::instance()->Create2dImage(fileType);
		if (pRawImage != NULL) // is this file a BMP?
		{
			pRawImage->ParseStream(textureDataStream);
			BuildTexture(textId, pRawImage);
			// free allocated data from the heap
			// this must also free the array of bmp pixels 
			delete pRawImage;
		}
		// free file content buffer
		delete[] textureDataStream;
	}
	else
	{
		DEBUG_OUT("Texture %s not found\n", textId);
	}
	
	// time measurement
	printf(" loading tex [%s] %.2fms\n", textId.data(), (float)(clock() - start));
}

void CTextureHolder::BuildTexture(const string textureId, const I2dImage* pData)
{
	const int wrap = 0;
	// allocate a texture name
	GLuint GeneratedTexture = -1;
	// [BEGIN] CRITICAL AREA

	// then, allocate an indexer for new texture
	glGenTextures(1, &GeneratedTexture);
	// binds this texture as a 2D bitmap
	glBindTexture(GL_TEXTURE_2D, GeneratedTexture);
	Int32 err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError BindTexture=%d\n", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	// always good to get the error in case it happens
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError Texture parameters=%d\n", err);
	}
	// build our texture mipmaps	
	if (pData->GetNumberOfBytes() == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			GL_RGBA, 
			pData->GetWidth(),
			pData->GetHeight(),
			0, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			pData->GetPointerToData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			GL_RGB, 
			pData->GetWidth(),
			pData->GetHeight(),
			0, 
			GL_RGB, 
			GL_UNSIGNED_BYTE, 
			pData->GetPointerToData());
	}
	// checks for GL ERROR
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("Error creating texture - glTexImage2D=%d\n", err);
		glDeleteTextures(1, &GeneratedTexture);
	}
	else
	{
		// adds to the database of textures
		m_textures.insert(make_pair(textureId, GeneratedTexture));
	}
		
}

void CTextureHolder::RemoveTexture(const string textId)
{
	TextureMap::iterator it = m_textures.find(textId);
	if (it == m_textures.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	glDeleteTextures(1, &(it->second));
	m_textures.erase(it);

	//_CrtDumpMemoryLeaks();
}

GLuint
CTextureHolder::getTextureById(string textId)
{
	// then try to find it in the textures map
	TextureMap::iterator result = m_textures.find(textId);
	if (result != m_textures.end())
	{
		return result->second;
	}

	// cache miss - then add this texture to the process list
	// TODO: make the loading asynchronous
	LoadTexture(textId);

	// if it somehow failed, returns -1
	return -1;
}

void 
CTextureHolder::AddTextureContent(string textId, Types::Byte* data)
{
	m_textureContentMapMutex->mutexLock();
	m_textureContentMap.insert(make_pair(textId, data));
	m_textureContentMapMutex->mutexUnlock();
}

CTextureHolder::~CTextureHolder()
{
	while (!m_textures.empty()) 
	{		
		TextureMap::iterator it = m_textures.begin();
		glDeleteTextures(1, &(it->second));
		m_textures.erase(it);
	}		
}

