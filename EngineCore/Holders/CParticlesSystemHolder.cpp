#include "CParticlesSystemHolder.h"
#include <time.h>
#include <iostream>
#include <string>
#include "MutexFactory.h"

CParticlesSystemHolder* CParticlesSystemHolder::s_pInstance = NULL;

bool CParticlesSystemHolder::Create()
{
	return false;
}

CParticlesSystemHolder::~CParticlesSystemHolder()
{
    m_mapParticles = DisplayObject();
}

bool CParticlesSystemHolder::Create(const string& pathToModelFile)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CParticlesSystemHolder
		s_pInstance = new CParticlesSystemHolder(pathToModelFile);
		// tries to open the resource file - out of the constructor so errors may be reported
		if (s_pInstance != nullptr) /*&& (s_pInstance->m_modelFiles->VOpen()))*/
		{
			return true;
		}
	}
	return false;
}

void 
CParticlesSystemHolder::OnRemoveEvent(const string& removeItem)
{
	s_pInstance->RemoveParticle(removeItem);
}

CParticlesSystemHolder::CParticlesSystemHolder(const string& pathToResources)
	/*: m_modelFiles(new CResourceZipFile(pathToResources.data(), this->OnRemoveEvent))*/
{
	m_pParticlesContentMapMutex = MutexFactory::Instance().Create("ParticlesContentMapMutex");
}

shared_ptr<Graphics::CParticle> CParticlesSystemHolder::findParticleInDB(const std::string& id)
{
	Graphics::CParticle* pRet = nullptr;
	// update all animations related to each model
	for (auto& it : m_mapParticles)
	{
		if (it.first == id)
		{
			return it.second;
		}
	}
	return nullptr;
}


void 
CParticlesSystemHolder::RemoveParticle(const string& textId)
{
	DisplayObject::iterator it = m_mapParticles.find(textId);
	if (it != m_mapParticles.end())
	{
        m_mapParticles.erase(it);
	}
}

shared_ptr<Graphics::IDrawable>
CParticlesSystemHolder::GetParticleById(const string& particleId)
{
	// then try to find it in the textures map
	DisplayObject::iterator result = m_mapParticles.find(particleId);
	if (result != m_mapParticles.end())
	{
		return result->second;
	}
	else
	{
		// cache miss - then create a particle system
		shared_ptr<Graphics::CParticle> pParticle = make_shared<Graphics::CParticle>(particleId);
		pParticle->Create();
		m_mapParticles.insert(make_pair(particleId, pParticle));
        return pParticle;
	}
}

void CParticlesSystemHolder::Update(float dt)
{
	// update all animations related to each model
	for (auto& it : m_mapParticles) 
	{		
		it.second->Update(dt);
	}
}
