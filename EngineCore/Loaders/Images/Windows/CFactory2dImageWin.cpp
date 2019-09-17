#include "CFactory2dImageWin.h"
#include "Bitmap.h"

void CFactory2dImageWin::Initialize()
{
	if (s_pInstance != nullptr)
	{
		// must delete to avoid memory leak
		// delete s_pInstance;
	}

	s_pInstance = new CFactory2dImageWin();
}

I2dImage* CFactory2dImageWin::Create2dImage(Byte fileType[])
{
	// default value is NULL
	I2dImage* retVal = new Bitmap();

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
