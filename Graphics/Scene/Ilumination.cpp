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
	for (auto it = m_lights.begin(); it != m_lights.end(); it++)
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

void Graphics::Ilumination::IncreaseAttenuationBy(const std::string& id, const float attenuationIncrease)
{
	for (auto it = m_lights.begin(); it != m_lights.end(); it++)
	{
		// de-reference the iterator to the list type
		Graphics::IluminationItem* pLightObj = *it;
		// calls overloaded operator==() for comparisson
		if (*pLightObj == id)
		{
			pLightObj->IncreaseAttenuation(attenuationIncrease);
			break;
		}
	}
}

void Graphics::Ilumination::SetLightColor(const std::string& id, const IvVector3& lightColor)
{
	for (auto it = m_lights.begin(); it != m_lights.end(); it++)
	{
		// de-reference the iterator to the list type
		Graphics::IluminationItem* pLightObj = *it;
		// calls overloaded operator==() for comparisson
		if (*pLightObj == id)
		{
			pLightObj->SetLightColor(lightColor);
			break;
		}
	}
}

void Graphics::Ilumination::SetAmbientLightColor(const IvVector3& lightColor)
{
	m_ambientColor = lightColor;
}

void Graphics::Ilumination::Remove(const std::string & id)
{
	for (auto it = m_lights.begin(); it != m_lights.end(); it++)
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
	for (auto it = m_lights.begin(); it != m_lights.end(); it++)
	{
		Graphics::IluminationItem* pLightObj = *it;
		if (*pLightObj == id)
		{
			location = pLightObj->GetLocation();
			return;
		}
	}
}

void Graphics::Ilumination::UpdateModel(const IvMatrix44 & model)
{
    m_pShadows->SetModel(model);
}

void Graphics::Ilumination::HasAnimations(bool value)
{
	// update the boolean flag for "has shadows"
	m_pShadows->HasAnimations(value);
}

float Graphics::Ilumination::GetLightAttenuation() const
{
	if (m_lights.size() > 0U)
		return (*m_lights.begin())->GetLightAttenuation();
	return 1.f;
}

const IvVector3& Graphics::Ilumination::GetLightColor() const
{
	if (m_lights.size() > 0U)
		return (*m_lights.begin())->GetLightColor();
	return IvVector3(1, 1, 1);
}

const IvVector3& Graphics::Ilumination::GetAmbientLightColor() const
{
	return m_ambientColor;
}

void Graphics::Ilumination::UpdateBoneTransformations(Float* boneTransformMat44, UInt32 boneIndex)
{
	if (m_pShadows)
		m_pShadows->UpdateBoneTransformations(boneTransformMat44, boneIndex);
}

void Graphics::Ilumination::BindShadowTexture()
{
	if (m_pShadows)
		m_pShadows->BindShadowTexture();
}

UInt32 Graphics::Ilumination::GetShadowTexture()
{
	if (m_pShadows)
		return m_pShadows->GetDepthMapId();
	return 0;
}


Graphics::Ilumination& Graphics::Ilumination::Instance()
{
	static Graphics::Ilumination instance;
	return instance;
}
