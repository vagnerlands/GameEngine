#ifndef _ITEXTURE_H_
#define _ITEXTURE_H_

#include "CommonTypes.h"

class I2dImage;

using namespace Types;

namespace Graphics
{

	class ITexture
	{
	public:
		ITexture() : m_status(false), m_textureId(0)
		{

		}
		virtual bool BuildTexture(const I2dImage* pData)
		{
			// must be implemented
			return false;
		}
		virtual bool BuildVectorTexture(const I2dImage** pData)
		{
			// no real implementation
			return false;
		}


		virtual bool Bind() = 0;
		virtual bool BindTextureToSampler(UInt32 samplerId) = 0;
		bool ProcessStatus()
		{
			return m_status;
		}

		virtual void Destroy()
		{
			delete this;
		}

		virtual ~ITexture()
		{
			// not implemented here
		}
	protected:
		UInt32 m_textureId;
		bool m_status;
	};

}
#endif // _ITEXTURE_H_