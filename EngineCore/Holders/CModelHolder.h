#ifndef CMODELMANAGER_H_
#define CMODELMANAGER_H_

#include "CommonTypes.h"
#include "OGLTypes.h"
#include "CResHandle.h"
#include "IMutex.h"
#include <unordered_map>
#include <memory>
#include "gl/glut.h"
#include "IModel.h"

using namespace Types;
using namespace std;

class CModelHolder
{
public:

	//typedef unordered_map<string, SModelData> ModelMap;
	typedef unordered_map<string, Graphics::IModel*> ModelObject;

	// always create before using
	static bool Create(const string& pathToModelFile);
	virtual ~CModelHolder();
	
	Graphics::IModel* LoadModel(const string& modelId);
    
	void RemoveModel(const string& modelId);
	Graphics::IModel* GetModelById(const string& modelId);

	void Update(float dt);

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(const string& removeItem);
	// local instance
	static CModelHolder* s_pInstance;

private:
	CModelHolder(const string& pathToResources);

	// hash map for the raw data models (vertexes, normals, material, etc)
	//ModelMap m_models;
	// hash map for the IModels* - containing what you need to render this model
	ModelObject m_mapModels;
	// mutex for m_processes
	IMutex* m_pModelContentMapMutex;

	// file content may be found here
	//IResourceFile* m_modelFiles;
	// cache database (allocated with fixed and known size)
	//CResCache m_cacheDb;


};

#endif //CMODELMANAGER_H_