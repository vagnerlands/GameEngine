#include "CTextureHolder.h"
#include "gl/glut.h"
#include <time.h>
#ifdef WIN32
#include "CWinMutex.h"
#include "CTextureOGL.h"
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
		Byte fileType[3];
		memcpy(&fileType, textureDataStream, 3);
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

	Graphics::ITexture* pTextureObj = NULL;
	if (pData != NULL)
	{
		pTextureObj = new Graphics::CTextureOGL();
		pTextureObj->BuildTexture(pData);
		if (pTextureObj->ProcessStatus())
		{
			m_textures.insert(make_pair(textureId, pTextureObj));
		}
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
	it->second->Destroy();
	m_textures.erase(it);

	//_CrtDumpMemoryLeaks();
}

Graphics::ITexture*
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
	return NULL;
}

void 
CTextureHolder::AddTextureContent(string textId, Types::Byte* data)
{
	m_textureContentMapMutex->mutexLock();
	m_textureContentMap.insert(make_pair(textId, data));
	m_textureContentMapMutex->mutexUnlock();
}

bool CTextureHolder::Bind(const string texId) const
{
	// then try to find it in the textures map
	TextureMap::const_iterator result = m_textures.find(texId);
	if (result != m_textures.end())
	{
		return result->second->Bind();
	}
	return false;
}

CTextureHolder::~CTextureHolder()
{
	while (!m_textures.empty()) 
	{		
		TextureMap::iterator it = m_textures.begin();
		it->second->Destroy();
		m_textures.erase(it);
	}		
}

