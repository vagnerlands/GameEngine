#ifndef _CTEXTUREOGL_H_
#define _CTEXTUREOGL_H_

#include "CommonTypes.h"
#include "ITexture.h"

class I2dImage;

using namespace Types;

namespace Graphics
{

	class CTextureOGL : public ITexture
	{
	public:
		virtual bool BuildTexture(const std::shared_ptr<I2dImage>& pData) override;
		virtual bool Bind() override;
		virtual bool BindTextureToSampler(UInt32 samplerId) override;

		virtual ~CTextureOGL();
	};

}
#endif // _CTEXTUREOGL_H_