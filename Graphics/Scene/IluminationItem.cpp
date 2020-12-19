#include "IluminationItem.h"

Graphics::IluminationItem::IluminationItem(const std::string id, IvVector3 & location, ELightType type)
	: m_id(id), m_location(location), m_type(LightType_Omni), m_lightAttenuation(0.05f), m_lightColor(1.f,1.f,1.f)
{
}

void Graphics::IluminationItem::SetLocation(const IvVector3 & location)
{
	m_location = location;
}

void Graphics::IluminationItem::SetDirection(ELightType type)
{
	m_type = type;
}
