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
	while (!m_mapParticles.empty())
	{
		if (m_mapParticles.begin()->second != NULL)
			delete m_mapParticles.begin()->second;
		m_mapParticles.erase(m_mapParticles.begin());
	}	
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

Graphics::CParticle* CParticlesSystemHolder::findParticleInDB(const std::string& id)
{
	Graphics::CParticle* pRet = nullptr;
	// update all animations related to each model
	for (auto& it : m_mapParticles)
	{
		if (it.first == id)
		{
			pRet = it.second;
		}
	}
	return pRet;
}


void 
CParticlesSystemHolder::RemoveParticle(const string& textId)
{
	DisplayObject::iterator it = m_mapParticles.find(textId);
	if (it == m_mapParticles.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	if (it->second != NULL)
	{
		delete it->second;
	}

	m_mapParticles.erase(it);

	//_CrtDumpMemoryLeaks();
}

Graphics::IDrawable*
CParticlesSystemHolder::GetParticleById(const string& particleId)
{
	Graphics::IDrawable* pRet = (Graphics::IDrawable*)(NULL);
	// then try to find it in the textures map
	DisplayObject::iterator result = m_mapParticles.find(particleId);
	if (result != m_mapParticles.end())
	{
		pRet = result->second;
	}
	else
	{
		// cache miss - then create a particle system
		Graphics::CParticle * pParticle = new Graphics::CParticle(particleId);
		pParticle->Create();
		m_mapParticles.insert(make_pair(particleId, pParticle));
		pRet = pParticle;
	}

	// if it somehow failed, returns -1
	return pRet;
}

void CParticlesSystemHolder::Update(float dt)
{
	// update all animations related to each model
	for (auto& it : m_mapParticles) 
	{		
		it.second->Update(dt);
	}
}

//void CParticlesSystemHolder::SetLoopCount(const string& particleId, Int32 loopCount)
//{
//	Graphics::CParticle* pParticle = findParticleInDB(particleId);
//	if (pParticle != nullptr)
//	{
//
//	}
//}
//
//void CParticlesSystemHolder::SetMaxNumberOfParticles(const string& particleId, Int32 maxNumberOfParticles)
//{
//	Graphics::CParticle* pParticle = findParticleInDB(particleId);
//	if (pParticle != nullptr)
//	{
//
//	}
//}
//
//void CParticlesSystemHolder::SetVariants(const string& particleId, Int32 spread, Int32 speed, Int32 height, Int32 age, Int32 size)
//{
//	Graphics::CParticle* pParticle = findParticleInDB(particleId);
//	if (pParticle != nullptr)
//	{
//
//	}
//}
//
//void CParticlesSystemHolder::SetParticlesPosition(const string& particleId, IvVector3& position)
//{
//	Graphics::CParticle* pParticle = findParticleInDB(particleId);
//	if (pParticle != nullptr)
//	{
//
//	}
//}
//
//void CParticlesSystemHolder::SetTexture(const string& particleId, const string& textureId)
//{
//	Graphics::CParticle* pParticle = findParticleInDB(particleId);
//	if (pParticle != nullptr)
//	{
//
//	}
//}
