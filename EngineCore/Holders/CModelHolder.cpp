#include "CModelHolder.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>

#include "MutexFactory.h"
#include "Model.h"
#include "IGame.h"
// should make this configurable - there should be a factory instead
#include "CModelOGL.h"

#include "Assimp\Importer.hpp"

// multi threading
#include "IMutex.h"
#include "CThreadHolder.h"
#include "IThread.h"

CModelHolder* CModelHolder::s_pInstance = NULL;

CModelHolder::~CModelHolder()
{
    // this clear all map models and release the shared_ptr counters of each item
    m_mapModels = ModelObject();
}

bool CModelHolder::Create(const string& pathToModelFile)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CModelHolder(pathToModelFile);
		// thread will start to run immediatelly
		CThreadHolder::instance()->registerThread("thModelLoading", 0x01, CModelHolder::Loading);
		// tries to open the resource file - out of the constructor so errors may be reported
		if (s_pInstance != nullptr) 
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
	while (EngineCore::IGame::mGame->IsRunning())
	{
		_Utils::JobList& jobs = s_pInstance->m_jobs;
		if (!jobs.IsEmpty())
		{
			_Utils::Job next(jobs);
			jobs.Retrieve(next);
			// parse the huge model files
            shared_ptr<Graphics::IModel> result = s_pInstance->LoadModel(next.Get());
			// push the results back to the job list, so it will 
			jobs.PushResult(result);
		}
	}
	// notify that this thread is closed
	EngineCore::IGame::mGame->IsReadyToClose();
}

CModelHolder::CModelHolder(const string& pathToResources)
	/*: m_modelFiles(new CResourceZipFile(pathToResources.data(), this->OnRemoveEvent))*/
{
	m_pModelContentMapMutex = MutexFactory::Instance().Create("ModelContentMap");
}

shared_ptr<Graphics::IModel>
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
	if (it != m_mapModels.end())
	{
        m_mapModels.erase(it);
	}    
}

shared_ptr<Graphics::IModel>
CModelHolder::GetModelById(const string& modelId)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		return result->second;
	}
	else
	{
		// cache miss - then add this texture to the process list
		// first, create it and push to the map models
		// must manipulate this object using specific CModelHolder
		// in the future, this should be a #define or a strategy
        shared_ptr<Graphics::IModel> pRet = make_shared<Graphics::CModelOGL>(modelId);
		m_mapModels.insert(make_pair(modelId, pRet));

		//pRet = LoadModel(modelId);
		m_jobs.PushRequest(modelId);
        return pRet;
	}
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
    bool finished = false;
	do
	{
        shared_ptr<Graphics::IModel> pModel = m_jobs.GetNextResult();
		if (pModel != nullptr)
		{
			// forces to use the already existing Model
			pModel->Apply(nullptr);
		}
        else
        {
            finished = true;
        }
	} while (!finished);
}

