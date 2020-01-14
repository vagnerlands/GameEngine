#include "CModelHolder.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>

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
	m_pModelContentMapMutex = MutexFactory::Instance().Create("ModelContentMap");
	if (m_pModelContentMapMutex == NULL)
	{
        DEBUG_OUT("Failed to create Mutex");
	}
}

Graphics::IModel*
CModelHolder::LoadModel(const string& modelId)
{
	// start loading measuring time
	clock_t start = clock();

	// must manipulate this object using specific CModelHolder
	// in the future, this should be a #define or a strategy
	Graphics::CModelOGL* pGfxModel = new Graphics::CModelOGL(modelId);

	//Model modelLoader(bytesStream, length);
	Model modelLoader;
	// actually tries to load the model
	modelLoader.Load("../Game/Assets/" + modelId);
	// set shader according to model loader results
	pGfxModel->SetShader(modelLoader.GetShaderSuggestion());

	// transfer this model data to the GPU
	pGfxModel->Create(modelLoader);

	m_mapModels.insert(make_pair(modelId, pGfxModel));

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));

	return pGfxModel;
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

Graphics::IModel*
CModelHolder::GetModelById(const string& modelId)
{
	Graphics::IModel* pRet = (Graphics::IModel*)(NULL);
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		pRet = result->second;
	}
	else
	{
		// cache miss - then add this texture to the process list
		pRet = LoadModel(modelId);
	}

	// if it somehow failed, returns -1
	return pRet;
}

void CModelHolder::DrawModelById(const string& modelId)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		result->second->Draw(false);
	}
	else
	{
		// cache miss - then add this texture to the process list
		LoadModel(modelId);
	}
}
