#include "CModelHolder.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>

#include <objParser/objLoader.h>
#include "MutexFactory.h"
#include "Model.h"
// should make this configurable - there should be a factory instead
#include "CModelOGL.h"

#include "Assimp\Importer.hpp"

CModelHolder* CModelHolder::s_pInstance = NULL;

CModelHolder::~CModelHolder()
{
	while (!m_mapModels.empty())
	{
		if (m_mapModels.begin()->second != NULL)
			delete m_mapModels.begin()->second;
		m_mapModels.erase(m_mapModels.begin());
	}	
}

bool CModelHolder::Create(const string& pathToModelFile)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CModelHolder(pathToModelFile);
		// tries to open the resource file - out of the constructor so errors may be reported
		if ((s_pInstance != nullptr) && (s_pInstance->m_modelFiles->VOpen()))
		{
			return true;
		}
	}
	return false;
}

void 
CModelHolder::OnRemoveEvent(const string& removeItem)
{
	s_pInstance->RemoveModel(removeItem);
}

CModelHolder::CModelHolder(const string& pathToResources)
	: m_modelFiles(new CResourceZipFile(pathToResources.data(), this->OnRemoveEvent))
{
	m_pModelContentMapMutex = MutexFactory::Instance().Create("ModelCOntentMap");
	if (m_pModelContentMapMutex == NULL)
	{
        DEBUG_OUT("Failed to create Mutex");
	}
}

void 
CModelHolder::LoadModel(const string& modelId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resModel(modelId);
	//Byte* modelStream = m_modelFiles->VAllocateAndGetResource(resModel);

	// checks if return value isn't null
	//if (modelStream != 0)
	{
		// material stream 
		//string materialId = modelId.substr(0, modelId.find_last_of('.'));
		//materialId += ".mtl";
		//CResource resMaterial(materialId);
		// tries to get the material data
		//Byte* materialStream = m_modelFiles->VAllocateAndGetResource(resMaterial);
        UInt32 modelSize = m_modelFiles->VGetResourceSize(resModel);
		// finally, perform allocation
        AddModelContent(modelId, nullptr, modelSize);
		//AddModelContent(modelId, modelStream, materialStream);

		// release resources
		//if (materialStream != 0)
		//{
		//	delete[] materialStream;
		//}
		//delete[] modelStream;
	}
/*	else
	{
		DEBUG_OUT("Model %s not found in resource file!\n", modelId);
	}*/	

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));
}

void
CModelHolder::AddModelContent(const string& modelId, Byte* bytesStream, UInt32 length)
{
    Graphics::IModel* pModelObj = new Graphics::CModelOGL();

    //Model modelLoader(bytesStream, length);
    Model modelLoader("../Game/Assets/" + modelId);

    pModelObj->Create(modelLoader);

    m_mapModels.insert(make_pair(modelId, pModelObj));
}

void 
CModelHolder::RemoveModel(const string& textId)
{
	ModelObject::iterator it = m_mapModels.find(textId);
	if (it == m_mapModels.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	if (it->second != NULL)
	{
		delete it->second;
	}

	m_mapModels.erase(it);

	//_CrtDumpMemoryLeaks();
}

Graphics::IModel&
CModelHolder::GetModelById(const string& modelId)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		return *result->second;
	}

	// cache miss - then add this texture to the process list
	LoadModel(modelId);
	
	// if it somehow failed, returns -1
	return *(Graphics::IModel*)(NULL);
}

void CModelHolder::DrawModelById(const string& modelId, cwc::glShader* shader)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		result->second->Draw(shader);
	}
	else
	{
		// cache miss - then add this texture to the process list
		LoadModel(modelId);
	}
}
