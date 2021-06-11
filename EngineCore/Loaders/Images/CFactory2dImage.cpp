#include "CFactory2dImage.h"
#include "CBmp.h"
#include "CTga.h"
#include "CPng.h"

CFactory2dImage* CFactory2dImage::s_pInstance = nullptr;

CFactory2dImage* CFactory2dImage::instance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new CFactory2dImage();
	}

	return s_pInstance;
}

std::shared_ptr<I2dImage>
CFactory2dImage::Create2dImage(Byte fileType[])
{
	if ((fileType[0] == 0x42) 
		&& (fileType[1] == 0x4D)) // BMP
	{
		return std::shared_ptr<I2dImage>(new CBmp);
	}
	else if ((fileType[1] == 'P') // TGA, no best way to identify the format type than this
		&& (fileType[2] == 'N'))
	{
        return std::shared_ptr<I2dImage>(new CPng);
	}
	else if ((fileType[0] == 0x00) // TGA, no best way to identify the format type than this
		&& (fileType[1] == 0x00)) 
	{
		return std::shared_ptr<I2dImage>(new CTga);
	}
}

CFactory2dImage::CFactory2dImage()
{
	// no implementation
}

CFactory2dImage::~CFactory2dImage()
{
	delete s_pInstance;
}
