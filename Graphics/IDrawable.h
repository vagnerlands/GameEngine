#ifndef _IDrawable_H_
#define _IDrawable_H_

#include "CommonTypes.h"
#include "IvVector3.h"
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
		{

		}
		// virtual destructor
		virtual ~IDrawable()
		{

		}
		// render this object as is
		virtual void Draw() = 0;

		virtual void SetLocation(const IvVector3 & newLocation)
		{
			m_location = newLocation;
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
		// identification
		std::string		m_id;
		IvVector3		m_location;

	private:
		// copy operations
		IDrawable(const IDrawable& other);
		IDrawable& operator=(const IDrawable& other);
	};


}
#endif // _IDrawable_H_