#ifndef _CTEXTUREOGL_H_
#define _CTEXTUREOGL_H_

#include "CommonTypes.h"
#include "ITexture.h"
#include "I2dImage.h"

using namespace Types;

namespace Graphics
{

	class CTextureOGL : public ITexture
	{
	public:
		virtual bool BuildTexture(const I2dImage* pData) override;
		virtual bool Bind() override;
		virtual bool BindTextureToSampler(UInt32 samplerId) override;

		virtual ~CTextureOGL();
	};

}
#endif // _CTEXTUREOGL_H_