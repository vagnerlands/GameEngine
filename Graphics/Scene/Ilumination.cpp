#include "Ilumination.h"

void Graphics::Ilumination::Add(IluminationItem * pLightSource)
{
	m_lights.push_back(pLightSource);
}

void Graphics::Ilumination::Update(const std::string & id, const IvVector3 & newLocation)
{
	for (std::list<Graphics::IluminationItem*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		Graphics::IluminationItem* pLightObj = *it;
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
		Graphics::IluminationItem* pLightObj = *it;
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

Graphics::Ilumination& Graphics::Ilumination::Instance()
{
	static Graphics::Ilumination instance;
	return instance;
}
