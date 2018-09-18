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
	~CModelHolder();
	static CModelHolder* instance();
	void LoadModel(const string modelId);
	void RemoveModel(const string modelId);
	bool getModelById(string modelId, SModelData& out);

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);


private:
	CModelHolder();
	void AddModelContent(string modelId, Byte* bytesStream, Byte* materialStream);
	// local hashmap built textures
	ModelMap m_models;
	// mutex for m_processes
	IMutex* m_pModelContentMapMutex;

	// file content may be found here
	CResourceZipFile m_modelFiles;
	// cache database (allocated with fixed and known size)
	CResCache m_cacheDb;
	// local instance
	static CModelHolder* s_pInstance;

};

#endif //CMODELMANAGER_H_