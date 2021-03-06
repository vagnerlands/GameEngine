#ifndef _CPNG_H_
#define _CPNG_H_

#include "I2dImage.h"
// released png methods header
#include <png.h>

using namespace Types;
using namespace std;


class CPng : public I2dImage
{
public:
	CPng();

    CPng(CPng&& rhs) : I2dImage(std::move(rhs))
    {}

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~CPng();

private:
	static void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
		png_size_t byteCountToRead);
	
};

#endif //_CPNG_H_