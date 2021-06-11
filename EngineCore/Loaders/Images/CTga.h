#ifndef _CTGA_H_
#define _CTGA_H_

#include "I2dImage.h"

using namespace Types;
using namespace std;

enum ETGATypes
{
	TGA_NODATA = 0,
	TGA_INDEXED = 1,
	TGA_RGB = 2,
	TGA_GRAYSCALE = 3,
	TGA_INDEXED_RLE = 9,
	TGA_RGB_RLE = 10,
	TGA_GRAYSCALE_RLE = 11
};

#pragma pack(1) 
typedef struct
{
	UByte    idLength;
	UByte    colorMapType;
	UByte    imageTypeCode;
	Int16    colourMapOrigin;
	Int16    colourMapLength;
	Byte     mapEntrySize;
	Int16    xOrigin;
	Int16    yOrigin;
	Int16    imageWidth;
	Int16    imageHeight;
	UByte    bitCount;
	UByte    imageDesc;

	/*Byte imageHeader[3];
	Int16 imageWidth;
	Int16 imageHeight;
	UByte bitCount;
	UByte* imageData;*/
} STgaFormat;
#pragma pack(pop)

class CTga : public I2dImage
{
public:
	CTga();

    CTga(CTga&& rhs) : I2dImage(std::move(rhs))
    {}

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~CTga();
	
};

#endif //_CTGA_H_