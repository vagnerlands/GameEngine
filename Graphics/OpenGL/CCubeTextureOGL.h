#ifndef _CCUBETEXTUREOGL_H_
#define _CCUBETEXTUREOGL_H_

#include "CommonTypes.h"
#include "ITexture.h"

class I2dImage;

using namespace Types;

namespace Graphics
{

	class CCubeTextureOGL : public ITexture
	{
	public:
		virtual bool BuildVectorTexture(const std::vector< std::shared_ptr<I2dImage> >& pData) override;
		virtual bool Bind() override;
		virtual bool BindTextureToSampler(UInt32 samplerId) override;

		virtual ~CCubeTextureOGL();
	};

}
#endif // _CCUBETEXTUREOGL_H_