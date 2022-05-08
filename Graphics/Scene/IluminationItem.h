#pragma once

#include "CommonTypes.h"
#include "IvVector3.h"

using namespace Types;

namespace Graphics
{
	enum ELightType
	{
		LightType_Omni = 0,
		LightType_Directional,
		LightType_Total
	};
	// Everything that can be rendered shall be a "IDrawable"
	class IluminationItem
	{
	public:
		IluminationItem(const std::string id, IvVector3&& location, ELightType type);

		// virtual destructor
		virtual ~IluminationItem()
		{

		}

		// for comparison
		virtual bool operator==(const IluminationItem& other)
		{
			return other.m_id == m_id;
		}

		// for comparison
		virtual bool operator==(const std::string& otherid)
		{
			return otherid == m_id;
		}

		virtual void SetLocation(const IvVector3& location);

		virtual void SetDirection(ELightType type);

		void IncreaseAttenuation(float increaseValue);

		void SetLightColor(const IvVector3& lightColor);

		virtual const IvVector3& GetLocation() const;

		virtual ELightType GetDirection() const;

		Float GetLightAttenuation() const;

		const IvVector3& GetLightColor() const;

	protected:
		IvVector3	m_location;
		ELightType	m_type;
		std::string m_id;
		float		m_lightAttenuation;
		IvVector3   m_lightColor;

	private:
		// copy operations
		IluminationItem(const IluminationItem& other) = delete;
		IluminationItem& operator=(const IluminationItem& other) = delete;
	};

	inline const IvVector3& IluminationItem::GetLocation() const
	{
		return m_location;
	}

	inline  ELightType IluminationItem::GetDirection() const
	{
		return m_type;
	}

	inline  Float IluminationItem::GetLightAttenuation() const
	{
		return m_lightAttenuation;
	}

	inline  const IvVector3& IluminationItem::GetLightColor() const
	{
		return m_lightColor;
	}

	inline void IluminationItem::IncreaseAttenuation(float increaseValue)
	{
		m_lightAttenuation += increaseValue;
		if (m_lightAttenuation < 0.f)
		{
			m_lightAttenuation = 0.f;
		}
        printf("[ DEBUG ] Light Attenuation value %.8f\n", m_lightAttenuation);
	}

	inline void IluminationItem::SetLightColor(const IvVector3& lightColor)
	{
		m_lightColor = lightColor;
	}

}