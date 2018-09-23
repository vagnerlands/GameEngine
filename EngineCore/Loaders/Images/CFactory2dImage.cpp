#include "CFactory2dImage.h"
#include "CBmp.h"

CFactory2dImage* CFactory2dImage::s_pInstance = nullptr;

CFactory2dImage* CFactory2dImage::instance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new CFactory2dImage();
	}

	return s_pInstance;
}

I2dImage* CFactory2dImage::Create2dImage(Int16 fileType)
{
	I2dImage* retVal = NULL;
	switch (fileType)
	{
		case 0x4D42:
			retVal = new CBmp();
			break;
		default:
			break;
	}

	return retVal;
}

CFactory2dImage::CFactory2dImage()
{
	// no implementation
}

CFactory2dImage::~CFactory2dImage()
{
	delete s_pInstance;
}
