#ifndef _ILight_H_
#define _ILight_H_

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
		IluminationItem(const std::string id, IvVector3& location, ELightType type);

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
		virtual bool operator==(const std::string& other)
		{
			return other == m_id;
		}

		virtual void SetLocation(const IvVector3& location);

		virtual void SetDirection(ELightType type);

		virtual const IvVector3& GetLocation() const;

		virtual ELightType GetDirection() const;

	protected:
		IvVector3	m_location;
		ELightType	m_type;
		std::string m_id;

	private:
		// copy operations
		IluminationItem(const IluminationItem& other);
		IluminationItem& operator=(const IluminationItem& other);
	};

	inline const IvVector3& IluminationItem::GetLocation() const
	{
		return m_location;
	}

	inline  ELightType IluminationItem::GetDirection() const
	{
		return m_type;
	}

}
#endif // _ILight_H_