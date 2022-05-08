#pragma once
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
		Ilumination() : m_ambientColor{0.15f, 0.15f, 0.15f}, m_pShadows{ nullptr }
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

		void IncreaseAttenuationBy(const std::string& id, const float attenuationIncrease);

		void SetLightColor(const std::string& id, const IvVector3& lightColor);

		void SetAmbientLightColor(const IvVector3& lightColor);

		// Update ilumination location
		virtual void Remove(const std::string& id);

		// Update ilumination location
		virtual void GetIluminationItemLocationPtr(const std::string& id, Float* location);
		virtual void GetIluminationItemLocation(const std::string& id, IvVector3& location);

        void UpdateModel(const IvMatrix44& model);

		void HasAnimations(bool value);

		float GetLightAttenuation() const;

		const IvVector3& GetLightColor() const;
		const IvVector3& GetAmbientLightColor() const;

		void UpdateBoneTransformations(Float* boneTransformMat44, UInt32 boneIndex);

        void BindShadowTexture();

        UInt32 GetShadowTexture();

		static Ilumination& Instance();

	protected:

		std::list<IluminationItem*> m_lights;

		Shadows* m_pShadows;

		IvVector3 m_ambientColor;

	private:

		// copy operations
		Ilumination(const Ilumination& other);
		Ilumination& operator=(const Ilumination& other);
	};


}