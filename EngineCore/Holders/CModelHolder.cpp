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

// multi threading
#include "IMutex.h"
#include "ThreadFactory.h"
#include "IThread.h"

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
		// instantiate a new thread
		s_pInstance->m_pThread = ThreadFactory::Instance().Create("ModelLoading", 0x01, CModelHolder::Loading);
		// tries to open the resource file - out of the constructor so errors may be reported
		if (s_pInstance != nullptr) /*&& (s_pInstance->m_modelFiles->VOpen()))*/
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

void CModelHolder::Loading()
{
	while (true)
	{
		_Utils::JobList& jobs = s_pInstance->m_jobs;
		if (!jobs.IsEmpty())
		{
			_Utils::Job next(jobs);
			jobs.Retrieve(next);
			// parse the huge model files
			Graphics::IModel* result = s_pInstance->LoadModel(next.Get());
			// push the results back to the job list, so it will 
			jobs.PushResult(result);
		}
	}
}

CModelHolder::CModelHolder(const string& pathToResources)
	/*: m_modelFiles(new CResourceZipFile(pathToResources.data(), this->OnRemoveEvent))*/
{
	m_pModelContentMapMutex = MutexFactory::Instance().Create("ModelContentMap");
}

Graphics::IModel*
CModelHolder::LoadModel(const string& modelId)
{
	// start loading measuring time
	clock_t start = clock();

	// then try to find it in the textures map
	ModelObject::iterator pGfxModel = m_mapModels.find(modelId);
	if (pGfxModel != m_mapModels.end())
	{
		// Try to Load
		(*pGfxModel).second->Create();
	}

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));

	return (*pGfxModel).second;
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
	Graphics::IModel* pRet = nullptr;
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		pRet = result->second;
	}
	else
	{
		// cache miss - then add this texture to the process list
		// first, create it and push to the map models
		// must manipulate this object using specific CModelHolder
		// in the future, this should be a #define or a strategy
		pRet = new Graphics::CModelOGL(modelId);
		m_mapModels.insert(make_pair(modelId, pRet));

		//pRet = LoadModel(modelId);
		m_jobs.PushRequest(modelId);
	}

	// if it somehow failed, returns -1
	return pRet;
}

void CModelHolder::Update(float dt)
{
	// update all animations related to each model
	for (auto& it : m_mapModels) 
	{		
		it.second->Update(dt);
	}
}

void CModelHolder::Refresh()
{
	Graphics::IModel* pModel = m_jobs.GetNextResult();
	if (pModel != nullptr)
	{
		// forces to use the already existing Model
		pModel->Apply(nullptr);
	}
}

