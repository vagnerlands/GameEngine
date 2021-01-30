#ifndef _SceneQuery_H_
#define _SceneQuery_H_

#include "CommonTypes.h"

using namespace Types;

namespace Graphics
{
	class ParticleSeeds
	{
	public:
		ParticleSeeds() : 
			IsValid(false), NumberOfParticles(0), SpeedMin(0), SpeedMax(0), SizeMin(0), SizeMax(0), HeightMin(0), HeightMax(0), SpreadMin(0), SpreadMax(0), AgeMin(0), AgeMax(0), Texture("flame.png")
		{}
		ParticleSeeds(Int32 numberofparticles,
			Int32 speedmin, Int32 speedmax,
			Int32 sizemin, Int32 sizemax,
			Int32 heightmin, Int32 heightmax,
			Int32 spreadmin, Int32 spreadmax,
			Int32 agemin, Int32 agemax,
			const Byte* texture) : 
			IsValid(true), NumberOfParticles(numberofparticles), SpeedMin(speedmin), SpeedMax(speedmax), SizeMin(sizemin), SizeMax(sizemax), HeightMin(heightmin), HeightMax(heightmax), SpreadMin(spreadmin), SpreadMax(spreadmax), AgeMin(agemin), AgeMax(agemax), Texture(texture)
		{}
	
		bool IsValid;
		Int32 NumberOfParticles;
		Int32 SpeedMin, SpeedMax;
		Int32 SizeMin, SizeMax;
		Int32 HeightMin, HeightMax;
		Int32 SpreadMin, SpreadMax;
		Int32 AgeMin, AgeMax;
		string Texture;
	};

	// Everything that can be rendered shall be a "IDrawable"
	class SceneQuery 
	{
	public:
		// virtual ctor
		SceneQuery()
		{

		}

		virtual ~SceneQuery()
		{

		}

		virtual const ParticleSeeds& GetParticlesSeeds() const
		{
			// makes clear that something went wrong in the casting
			static ParticleSeeds invalid;
			return invalid;
		}

	};
}
#endif // _SceneQuery_H_
