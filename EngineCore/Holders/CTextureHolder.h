#ifndef CTEXTMANAGER_H_
#define CTEXTMANAGER_H_

#include "CommonTypes.h"
#include "ITexture.h"
#include "OGLTypes.h"
#include "CResHandle.h"
#include "I2dImage.h"

#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class IMutex;

struct STextureItem
{
    STextureItem() : m_name(""), m_size(0U) {}
    STextureItem(string name, UInt32 size) : m_name(name), m_size(size) {}
    bool operator==(const STextureItem& other)
    {
        if (other.m_name == this->m_name)
            return true;
        else
            return false;
    }
    string                  m_name;
    UInt32                  m_size;
};

typedef list<STextureItem> TextureLru;

class CTextureHolder 
{
public:
	typedef unordered_map<string, Graphics::ITexture*> TextureMap;
	typedef unordered_map<string, Types::Byte*> TextureContentMap;

	static bool Create(const string& pathToTexturesFile, UInt32 maxAllocSize);
	void LoadTexture(const string& textId);
    std::shared_ptr<I2dImage> PrepareTexture(const string& textId);
	void RemoveTexture(const string& textId);
	Graphics::ITexture* getTextureById(const string& textId);
	Graphics::ITexture* getTextureVector(const vector<SModelTexture>& attr);
	bool Bind(const string& texId) const ;
	CTextureHolder::~CTextureHolder();
    void Update();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(const string& removeItem);
	// local instance
	static CTextureHolder* s_pInstance;


private:
	CTextureHolder(const string& pathToTexturesFile, UInt32 maxAllocSize);
	// build texture, if any available
	void BuildTexture(const string& textureId, const std::shared_ptr<I2dImage>& pData);
    void increaseTexturePriority(const string & textureId, UInt32 size);
    void removeLastItem();
	// local hashmap built textures
	TextureMap m_textures;
	// local hashmap for textures to be generated
	TextureContentMap m_textureContentMap;
	// mutex for m_processes
	IMutex* m_textureContentMapMutex;

	// resource database - TEXTURES
    TextureLru              m_lru;
	IResourceFile*          m_pResHandler;
    // reserved size for VRAM
    UInt32                  m_maxAllocSize;
    // total VRAM size in usage 
    UInt32                  m_sizeInUse;
    static const string     sc_invalidTextureName;
};

#endif //CTEXTMANAGER_H_