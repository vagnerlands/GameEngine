#ifndef _SceneQueryParticles_H_
#define _SceneQueryParticles_H_

#include "CommonTypes.h"
#include "Scene/Query/SceneQuery.h"

using namespace Types;

namespace Graphics
{

	// Everything that can be rendered shall be a "IDrawable"
	class SceneQueryParticles : public SceneQuery
	{
	public:
		// virtual ctor
		SceneQueryParticles() : SceneQuery()
		{
		}

		virtual ~SceneQueryParticles()
		{

		}

		void Set(const ParticleSeeds& seeds)
		{
			m_seeds = seeds;
		}

		virtual const ParticleSeeds& GetParticlesSeeds() const override
		{
			return m_seeds;
		}

	private:
		ParticleSeeds m_seeds;

	};
}
#endif // _SceneQueryParticles_H_
