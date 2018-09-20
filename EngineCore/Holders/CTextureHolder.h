#ifndef CTEXTMANAGER_H_
#define CTEXTMANAGER_H_

#include "CommonTypes.h"
#include "OGLTypes.h"
#include "CResHandle.h"
#include "IMutex.h"

#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class CTextureHolder 
{
public:
	static bool Create(const string pathToTexturesFile);
	void LoadTexture(const string textId);
	void RemoveTexture(const string textId);
	GLuint getTextureById(string textId);
	void AddTextureContent(string textId, Types::Byte* data);
	CTextureHolder::~CTextureHolder();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);

	// local instance
	static CTextureHolder* s_pInstance;


private:
	CTextureHolder(const string pathToTexturesFile);
	// build texture, if any available
	void BuildTexture();
	// local hashmap built textures
	TextureMap m_textures;
	// local hashmap for textures to be generated
	TextureContentMap m_textureContentMap;
	// mutex for m_processes
	IMutex* m_textureContentMapMutex;

	// resource database - TEXTURES
	IResourceFile* m_textureFiles;

};

#endif //CTEXTMANAGER_H_