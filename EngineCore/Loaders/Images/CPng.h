#ifndef _CPNG_H_
#define _CPNG_H_

#include "I2dImage.h"
// released png methods header
#include <png.h>

using namespace Types;
using namespace std;


class CPng : public I2dImage
{
    Byte* m_pImageDataBuffer;
    void copyPngDataToOutputBuffer(UInt32 imageWidth, UInt32 imageHeight, UInt32 pixelDataSize, Byte* pPixelData);
public:
	CPng();

    CPng(CPng&& rhs) : I2dImage(std::move(rhs))
    {}

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~CPng();

};

#endif //_CPNG_H_