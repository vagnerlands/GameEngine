#include "CFactory2dImageWin.h"
#include "DevILLoader.h"
#include "Bitmap.h"

void CFactory2dImageWin::Initialize()
{
	if (CFactory2dImage::s_pInstance != nullptr)
	{
		// changing the factory during the run, uh? Well, why not...
		delete CFactory2dImage::s_pInstance;
	}

    CFactory2dImage::s_pInstance = new CFactory2dImageWin();
}

std::shared_ptr<I2dImage> CFactory2dImageWin::Create2dImage(Byte fileType[])
{
	// default value is NULL
    return std::shared_ptr<I2dImage>(new DevILLoader);
}

CFactory2dImageWin::CFactory2dImageWin() : CFactory2dImage()
{
	// no implementation
}

CFactory2dImageWin::~CFactory2dImageWin()
{
	// calls father destructor
	CFactory2dImage::~CFactory2dImage();
}
