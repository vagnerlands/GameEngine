#include "Ilumination.h"

void Graphics::Ilumination::Initialize(Shadows* pShadows)
{
	m_pShadows = pShadows;
	m_pShadows->Initialize();
}

void Graphics::Ilumination::StartShadowsDepth()
{
	IvVector3 MainLight;
	GetIluminationItemLocation("main", MainLight);
	m_pShadows->Start(MainLight);
}

void Graphics::Ilumination::FinishShadowsDepth()
{
	m_pShadows->Stop();
}

void Graphics::Ilumination::Add(IluminationItem * pLightSource)
{
	m_lights.push_back(pLightSource);
}

void Graphics::Ilumination::Update(const std::string & id, const IvVector3 & newLocation)
{
	for (std::list<Graphics::IluminationItem*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		// de-reference the iterator to the list type
		Graphics::IluminationItem* pLightObj = *it;
		// calls overloaded operator==() for comparisson
		if (*pLightObj == id)
		{
			pLightObj->SetLocation(newLocation);
			break;
		}
	}
}

void Graphics::Ilumination::Remove(const std::string & id)
{
	for (std::list<Graphics::IluminationItem*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		// de-reference the iterator to the list type
		Graphics::IluminationItem* pLightObj = *it;
		// calls overloaded operator==() for comparisson
		if (*pLightObj == id)
		{
			m_lights.remove(pLightObj);
			delete pLightObj;
			break;
		}
	}
}

void Graphics::Ilumination::GetIluminationItemLocationPtr(const std::string & id, Float* location)
{
	for (std::list<Graphics::IluminationItem*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		Graphics::IluminationItem* pLightObj = *it;
		if (*pLightObj == id)
		{
			location[0] = pLightObj->GetLocation().GetX();
			location[1] = pLightObj->GetLocation().GetY();
			location[2] = pLightObj->GetLocation().GetZ();
			return;
		}
	}
}

void Graphics::Ilumination::GetIluminationItemLocation(const std::string & id, IvVector3 & location)
{
	for (std::list<Graphics::IluminationItem*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		Graphics::IluminationItem* pLightObj = *it;
		if (*pLightObj == id)
		{
			location = pLightObj->GetLocation();
			return;
		}
	}
}

Graphics::Ilumination& Graphics::Ilumination::Instance()
{
	static Graphics::Ilumination instance;
	return instance;
}
