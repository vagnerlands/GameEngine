#include "CTextureHolder.h"
#include "gl/glut.h"
#include <time.h>
#include "MutexFactory.h"
#include <iostream>
#ifdef WIN32
#include "CTextureOGL.h"
#include "CCubeTextureOGL.h"
#endif

#include "CFactory2dImage.h"

CTextureHolder* CTextureHolder::s_pInstance = NULL;

bool CTextureHolder::Create(const string& pathToTexturesFile, UInt32 maxAllocSize)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CTextureHolder(pathToTexturesFile, maxAllocSize);
		// tries to open the resource file - out of the constructor so errors may be reported
		if ((s_pInstance != nullptr) 
            && (s_pInstance->m_pResHandler->VOpen()))
		{
			return true;
		}
	}
	return false;
}

CTextureHolder::CTextureHolder(const string& pathToTexturesFile, UInt32 maxAllocSize) :
    m_pResHandler(new CResourceZipFile(pathToTexturesFile.data(), this->OnRemoveEvent)),
    m_maxAllocSize(maxAllocSize),
    m_sizeInUse(0U)
{
	// cross platform implementation for mutex creation
	m_textureContentMapMutex = MutexFactory::Instance().Create("TextureMapMutex");
}

void 
CTextureHolder::OnRemoveEvent(const string& removeItem)
{
	s_pInstance->RemoveTexture(removeItem);
}

void 
CTextureHolder::LoadTexture(const string& textId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resourceItem(textId);

	Byte*  textureDataStream = m_pResHandler->VAllocateAndGetResource(resourceItem);
	UInt32 textureDataLength = m_pResHandler->VGetResourceSize(resourceItem);
	// status OK
	if (textureDataStream != 0)
	{
		// checks the first 2 bytes of the stream to know if we know how to parse it
		Byte fileType[3];
		memcpy(&fileType, textureDataStream, 3);
		I2dImage* pRawImage = CFactory2dImage::instance()->Create2dImage(fileType);
		if (pRawImage != NULL) // is this file a BMP?
		{
			if (pRawImage->ParseStream(textureDataStream, textureDataLength))
			{
				BuildTexture(textId, pRawImage);
			}
			else
			{
				std::cout << " Bad input stream, failed to load texture [" << textId << "]" << std::endl;
			}
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

void CTextureHolder::BuildTexture(const string& textureId, const I2dImage* pData)
{

	Graphics::ITexture* pTextureObj = NULL;
	if (pData != NULL)
	{
		pTextureObj = new Graphics::CTextureOGL();
		pTextureObj->BuildTexture(pData);
		if (pTextureObj->ProcessStatus())
		{
			m_textures.insert(make_pair(textureId, pTextureObj));
            // TODO: make sure that the texture is unique in the lru database
            increaseTexturePriority(textureId, pData->GetSizeInBytes());
		}
	}		
}

void CTextureHolder::increaseTexturePriority(const string& textureId, UInt32 size = 0U)
{
	LockGuard lock(m_textureContentMapMutex);
    for (TextureLru::iterator it = m_lru.begin(); it != m_lru.end(); it++)
    {
        if ((*it).m_name == textureId)
        {
            // creates a copy of the iterator value
            STextureItem copyItem = *it;
            // remove the item from whichever place it's now
            m_lru.remove(*it);
            // and re-insert on top
            m_lru.push_front(copyItem);
            
			return;
        }
    }

    // create new record in LRU
    if (size > 0U)
    {
        while ((size + m_sizeInUse) > m_maxAllocSize)
        {
            removeLastItem();
        }
        m_sizeInUse += size;
        m_lru.push_front(STextureItem(textureId, size));
    }
}

void CTextureHolder::removeLastItem()
{
    STextureItem& doomed = m_lru.back();
    RemoveTexture(doomed.m_name);
    m_sizeInUse -= doomed.m_size;
    printf(" tex removed %s\n", doomed.m_name.c_str());
    m_lru.pop_back();
}

void CTextureHolder::RemoveTexture(const string& textId)
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
}

Graphics::ITexture*
CTextureHolder::getTextureById(const string& textId)
{
	// then try to find it in the textures map
	TextureMap::iterator result = m_textures.find(textId);
	if (result != m_textures.end())
	{
        // take the record from the lru and push on front
        increaseTexturePriority(textId);
		return result->second;
	}

	// cache miss - then add this texture to the process list
	// TODO: make the loading asynchronous
	LoadTexture(textId);

	// if it somehow failed, returns -1
	return nullptr;
}

Graphics::ITexture * CTextureHolder::getTextureVector(const vector<SModelTexture>& attr)
{
	// for every purpose, texture vectors are indexed by the first element
	// then try to find it in the textures map
	std::string textureId = attr[0].m_filename;
	TextureMap::iterator result = m_textures.find(textureId);
	if (result != m_textures.end())
	{
		// take the record from the lru and push on front
		increaseTexturePriority(textureId);
		return result->second;
	}

	// start loading measuring time
	clock_t start = clock();

	const UInt32 arrLen = attr.size();

	// cache missed - must reload it from resources db
	CResource* resourceItem = new CResource[arrLen];
	I2dImage** pRawImageArray = new I2dImage*[arrLen];
	for (UInt32 i = 0; i < arrLen; ++i)
	{
		resourceItem[i].SetName(attr[i].m_filename);
		Byte*  textureDataStream = m_pResHandler->VAllocateAndGetResource(resourceItem[i]);
		UInt32 textureDataLength = m_pResHandler->VGetResourceSize(resourceItem[i]);
		// checks the first 2 bytes of the stream to know if we know how to parse it
		Byte fileType[3];
		memcpy(&fileType, textureDataStream, 3);
		pRawImageArray[i] = CFactory2dImage::instance()->Create2dImage(fileType);

		if (pRawImageArray[i]->ParseStream(textureDataStream, textureDataLength))
		{

		}
		// free file content buffer
		delete[] textureDataStream;
	}

	// build 3D Cube texture data
	Graphics::ITexture* pTextureObj = NULL;
	//if (pData != NULL)
	{
		pTextureObj = new Graphics::CCubeTextureOGL();
		pTextureObj->BuildVectorTexture((const I2dImage**)pRawImageArray);
		if (pTextureObj->ProcessStatus())
		{
			m_textures.insert(make_pair(textureId, pTextureObj));
			// TODO: make sure that the texture is unique in the lru database
			increaseTexturePriority(textureId/*, pData->GetSizeInBytes()*/);
		}
	}

	


	// release allocated data
	for (UInt32 i = 0; i < arrLen; ++i)
	{
		delete pRawImageArray[i];
	}

	delete[] pRawImageArray;
	delete[] resourceItem;

	// time measurement
	printf(" loading cube tex [%s] %.2fms\n", textureId.data(), (float)(clock() - start));

	// if it somehow failed, returns -1
	return nullptr;
}

bool CTextureHolder::Bind(const string& texId) const
{
	// then try to find it in the textures map
	TextureMap::const_iterator result = m_textures.find(texId);
	if (result != m_textures.end())
	{
        // take the record from the lru and push on front
        s_pInstance->increaseTexturePriority(texId);
        // actually binds the texture
		return result->second->Bind();
	}
    else
    {
        // try to load the texture
        Graphics::ITexture* pText = s_pInstance->getTextureById(texId);
        if (pText != nullptr)
        {
            return pText->Bind();
        }
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

