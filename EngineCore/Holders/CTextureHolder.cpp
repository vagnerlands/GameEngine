#include "CTextureHolder.h"
#include "gl/glut.h"
#include <time.h>
#ifdef WIN32
#include "CWinMutex.h"
#endif

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
		AddTextureContent(textId, textureDataStream);
		// release allocated resources
		delete[] textureDataStream;
	}
	else
	{
		DEBUG_OUT("Texture %s not found\n", textId);
	}
	
	// time measurement
	printf(" loading tex [%s] %.2fms\n", textId.data(), (float)(clock() - start));
}

void CTextureHolder::BuildTexture()
{
	const int wrap = 0;
	// allocate a texture name
	GLuint GeneratedTexture = -1;
	// [BEGIN] CRITICAL AREA
	m_textureContentMapMutex->mutexLock();
	for (auto it = m_textureContentMap.begin(); !m_textureContentMap.empty();)
	{
		// whole data content of the image (texture)
		Byte* data = it->second;
		// retrieve header information
		// offset for start of bmp data
		Int32 dataPos = *(int*)&(data[0x0A]);
		// image size in bytes
		Int32 imageSize = *(int*)&(data[0x22]);
		// width in pixels
		Int32 width = *(int*)&(data[0x12]);
		// height in pixels
		Int32 height = *(int*)&(data[0x16]);
		// supports only 24bits and 32bits
		Int32 numOfBytesPerPixel = 3;
		// checks if this image is 32 bits
		if (imageSize == (width * height * 4))
			numOfBytesPerPixel = 4;

		// then, allocate an indexer for new texture
		glGenTextures(1, &GeneratedTexture);
		// binds this texture as a 2D bitmap
		glBindTexture(GL_TEXTURE_2D, GeneratedTexture);
		int err = glGetError();
		if (err != 0)
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
		if (err != 0)
		{
			printf("glError Texture parameters=%d\n", err);
		}
		// data must be sent in BGRA order - must swap byte order of integer
		Byte* swapBuffer = new Byte[width*height*numOfBytesPerPixel];
		for (Int32 x = 0; x < (width*height*numOfBytesPerPixel);)
		{
			if (numOfBytesPerPixel == 4)
			{
				swapBuffer[x] = data[x + dataPos + 2];
				swapBuffer[x + 1] = data[x + dataPos + 1];
				swapBuffer[x + 2] = data[x + dataPos + 0];
				swapBuffer[x + 3] = data[x + dataPos + 3];
				x += numOfBytesPerPixel;
			}
			if (numOfBytesPerPixel == 3)
			{
				swapBuffer[x] = data[x + dataPos + 2];
				swapBuffer[x + 1] = data[x + dataPos + 1];
				swapBuffer[x + 2] = data[x + dataPos];
				x += numOfBytesPerPixel;
			}
		}

		// build our texture mipmaps	
		if (numOfBytesPerPixel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, swapBuffer);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, swapBuffer);
		}
		// release temporary buffer for swap
		delete[] swapBuffer;
		// checks for GL ERROR
		err = glGetError();
		if (err != 0)
		{
			printf("glError glTexImage2D=%d\n", err);
		}
		// adds to the hashmap
		m_textures.insert(make_pair(it->first, GeneratedTexture));

		// deletes allocated memory (do not allow it to leak)
		delete[] it->second;
		// deletes the object from the DS
		m_textureContentMap.erase(it);
	}
	// [END] CRITICAL AREA
	m_textureContentMapMutex->mutexUnlock();

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
	// builds any texture it has to build first
	BuildTexture();
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

