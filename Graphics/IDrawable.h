#ifndef _IDrawable_H_
#define _IDrawable_H_

#include "CommonTypes.h"
#include "IvVector3.h"
#include "IvQuat.h"
#include "Shaders/glsl.h"

using namespace Types;

namespace Graphics
{
	// Everything that can be rendered shall be a "IDrawable"
	class IDrawable
	{
	public:
		IDrawable() : 
			//m_id(itoa(rand(),)),  // tries, vaguely, to create unique ids
			m_location(0.f, 0.f, 0.f) // default location
			, m_scale(1.f, 1.f, 1.f) // default scale (original size from editor)
            , m_hasShadow(true)
		{
			m_rotation.Identity();
		}
		// virtual destructor
		virtual ~IDrawable()
		{

		}
		virtual void Update(float dt) = 0;
		// render this object as is
		virtual void Draw(float dt, bool isRenderingShadows) = 0;

		virtual void SetLocation(const IvVector3 & newLocation)
		{
			m_location = newLocation;
		}

		virtual void SetScale(const IvVector3 & newDimension)
		{
			m_scale = newDimension;
		}

		virtual void SetRotation(const IvQuat & newRotation)
		{
			m_rotation = newRotation;
		}

		virtual void SetId(const std::string& id)
		{
			m_id = id;
		}

		virtual void SetHasShadow(bool hasShadow)
		{
			m_hasShadow = hasShadow;
		}

		// for comparison
		virtual bool operator==(const IDrawable& other)
		{
			return other.m_id == m_id;
		}
		// for comparison
		virtual bool operator==(const std::string& other)
		{
			return other == m_id;
		}

	protected:
		// identification of this drawable, user must set it
		std::string		m_id;
		// world location of this drawable
		IvVector3		m_location;
		// scale resize for the model
		IvVector3		m_scale;
		// rotation matrix for the model
		IvQuat			m_rotation;
		// this object blocks light?
		bool            m_hasShadow;

	private:
		// copy operations
		IDrawable(const IDrawable& other);
		IDrawable& operator=(const IDrawable& other);
	};


}
#endif // _IDrawable_H_