#ifndef _Shadows_H_
#define _Shadows_H_

#include "CommonTypes.h"

using namespace Types;

// forward declaration
class IvVector3;
class IvMatrix44;

namespace Graphics
{
	// Database of light sources
	class Shadows
	{
	public:
		Shadows()
		{

		}
		// virtual destructor
		virtual ~Shadows()
		{

		}

		virtual bool Initialize() = 0;

		virtual void Start(const IvVector3& lightPos) = 0;

		virtual void Stop() = 0;

		virtual UInt32 GetDepthMapId() const = 0;

        virtual void SetModel(const IvMatrix44& model) = 0;

		virtual void HasAnimations(bool value) = 0;

		virtual void UpdateBoneTransformations(Float* boneTransformMat44, UInt32 boneIndex) = 0;

        virtual void BindShadowTexture() = 0;

	protected:

	private:

		// copy operations
		Shadows(const Shadows& other);
		Shadows& operator=(const Shadows& other);
	};


}
#endif // _Shadows_H_