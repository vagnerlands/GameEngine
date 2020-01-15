#ifndef _Ilumination_H_
#define _Ilumination_H_

#include "CommonTypes.h"
#include "IluminationItem.h"
#include "Shadows.h"


using namespace Types;

namespace Graphics
{
	// Database of light sources
	class Ilumination
	{
	public:
		Ilumination()
		{


		}
		// virtual destructor
		virtual ~Ilumination()
		{

		}

		void Initialize(Shadows* pShadows);

		void StartShadowsDepth();
		void FinishShadowsDepth();

		// render this object as is
		virtual void Add(IluminationItem* pLightSource);

		// Update ilumination location
		virtual void Update(const std::string& id, const IvVector3& newLocation);

		// Update ilumination location
		virtual void Remove(const std::string& id);

		// Update ilumination location
		virtual void GetIluminationItemLocationPtr(const std::string& id, Float* location);
		virtual void GetIluminationItemLocation(const std::string& id, IvVector3& location);

		static Ilumination& Instance();

	protected:

		std::list<IluminationItem*> m_lights;

		Shadows* m_pShadows;

	private:

		// copy operations
		Ilumination(const Ilumination& other);
		Ilumination& operator=(const Ilumination& other);
	};


}
#endif // _Ilumination_H_