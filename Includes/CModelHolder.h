#ifndef CMODELMANAGER_H_
#define CMODELMANAGER_H_

#include "CommonTypes.h"
#include "OGLTypes.h"
#include "CResHandle.h"
#include "IMutex.h"
#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class CModelHolder
{
public:
	// always create before using
	static bool Create(const string pathToModelFile);
	virtual ~CModelHolder();
	
	void LoadModel(const string modelId);
	void RemoveModel(const string modelId);
	bool getModelById(const string modelId, SModelData& out);

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);
	// local instance
	static CModelHolder* s_pInstance;

private:
	CModelHolder(std::string pathToResources);
	void AddModelContent(string modelId, Byte* bytesStream, Byte* materialStream);
	// local hashmap built textures
	ModelMap m_models;
	// mutex for m_processes
	IMutex* m_pModelContentMapMutex;

	// file content may be found here
	IResourceFile* m_modelFiles;
	// cache database (allocated with fixed and known size)
	//CResCache m_cacheDb;


};

#endif //CMODELMANAGER_H_