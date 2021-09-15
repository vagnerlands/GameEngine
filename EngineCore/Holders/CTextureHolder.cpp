#include "CTextureHolder.h"
#include "gl/glut.h"
#include <time.h>
#include "MutexFactory.h"
#include <iostream>
#ifdef WIN32
#include "OpenGL/CTextureOGL.h"
#include "OpenGL/CCubeTextureOGL.h"
#endif
#include "Logger/ILogger.h"
#include "IMutex.h"
#include "CFactory2dImage.h"
#include "Utils/Jobs.h"

CTextureHolder* CTextureHolder::s_pInstance = NULL;
const string CTextureHolder::sc_invalidTextureName = "__no_valid_texture__.bmp";

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
            s_pInstance->LoadTexture(sc_invalidTextureName);
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

    LOG("Start loading [" + textId.c_str() + "]");

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
		std::shared_ptr<I2dImage> pRawImage = CFactory2dImage::instance()->Create2dImage(fileType);
		if (pRawImage != nullptr) // is this file a BMP?
		{
			if (pRawImage->ParseStream(textureDataStream, textureDataLength))
			{
				BuildTexture(textId, pRawImage);
			}
			else
			{
                LOG("Bad input stream, failed to load texture [" + textId.c_str() + "]");
			}
		}
	}
	else
	{
        LOG("Texture [" + textId.c_str() + "] not found");
	}

    if (textureDataStream != nullptr)
    {
        delete[] textureDataStream;
    }
	
	// time measurement
    LOG("Finished loading [" + textId.c_str() + "] in " + to_string((float)(clock() - start)) + "ms");
}

std::shared_ptr<I2dImage> CTextureHolder::PrepareTexture(const string & textId)
{
    std::shared_ptr<I2dImage> pRawImage = nullptr;
    // start loading measuring time
    clock_t start = clock();

    LOG("Start loading [" + textId.c_str() + "]");

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
        pRawImage = CFactory2dImage::instance()->Create2dImage(fileType);
        if (pRawImage != nullptr) // is this file a BMP?
        {
            if (!pRawImage->ParseStream(textureDataStream, textureDataLength))
            {
                pRawImage = nullptr;
                LOG("Bad input stream, failed to load texture [" + textId.c_str() + "]");
            }
        }
    }
    else
    {
        LOG("Texture [" + textId.c_str() + "] not found");
    }

    if (textureDataStream != nullptr)
    {
        delete[] textureDataStream;
    }

    // time measurement
    LOG("Finished loading [" + textId.c_str() + "] in " + to_string((float)(clock() - start)) + "ms");

    return pRawImage;
}

void CTextureHolder::BuildTexture(const string& textureId, const std::shared_ptr<I2dImage>& pData)
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
    LOG("Removed Texture "
        + doomed.m_name
        + " Current "
        + to_string(m_sizeInUse)
        + " (Free " + to_string((m_maxAllocSize - m_sizeInUse))
        + ") %" + to_string(((Float)m_sizeInUse / m_maxAllocSize)*100.f));
    m_lru.pop_back();
}

void CTextureHolder::RemoveTexture(const string& textId)
{
	TextureMap::iterator it = m_textures.find(textId);
	if (it != m_textures.end())
	{
        it->second->Destroy();
        m_textures.erase(it);
	}
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

    _Utils::Jobs::Instance().PushRequest([&](const string& id) {return s_pInstance->PrepareTexture(id); }, textId);

    // if it somehow failed, returns -1
    increaseTexturePriority(sc_invalidTextureName);
	return m_textures.find(sc_invalidTextureName)->second;
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

    LOG("Start loading 3D texture [" + textureId + "]");
	const UInt32 arrLen = attr.size();

	// cache missed - must reload it from resources db
	CResource* resourceItem = new CResource[arrLen];
    std::vector< std::shared_ptr<I2dImage> > pRawImageArray(arrLen);
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
		pTextureObj->BuildVectorTexture(pRawImageArray);
		if (pTextureObj->ProcessStatus())
		{
			m_textures.insert(make_pair(textureId, pTextureObj));
			// TODO: make sure that the texture is unique in the lru database
			increaseTexturePriority(textureId/*, pData->GetSizeInBytes()*/);
		}
	}
	delete[] resourceItem;

	// time measurement
    LOG("Finished loading [" + textureId + "] in " + to_string((float)(clock() - start)) + "ms");

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

void CTextureHolder::Update()
{
    // TODO: move this to another place
    auto next = _Utils::Jobs::Instance().GetNextResult();
    if (next.first != nullptr)
    {
        // extra check, it's possible that in between the thread executions, this job was already processed, so we have to discard before allocating 
        // graphic resources
        if (m_textures.find(next.second) == m_textures.end())
        {
            BuildTexture(next.second, next.first);
        }
    }
}

