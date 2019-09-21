#include "CFactory2dImageWin.h"
#include "DevILLoader.h"
#include "Bitmap.h"

void CFactory2dImageWin::Initialize()
{
	if (s_pInstance != nullptr)
	{
		// changing the factory during the run, uh? Well, why not...
		delete s_pInstance;
	}

	s_pInstance = new CFactory2dImageWin();
}

I2dImage* CFactory2dImageWin::Create2dImage(Byte fileType[])
{
	// default value is NULL
	I2dImage* retVal = new DevILLoader();

	return retVal;
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
