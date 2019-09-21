#ifndef _CFACTORY2DIMAGE_H_
#define _CFACTORY2DIMAGE_H_

#include "I2dImage.h"

using namespace Types;
using namespace std;

class I2dImage;

class CFactory2dImage 
{
public:
	
	static CFactory2dImage* instance();
	virtual I2dImage* Create2dImage(Byte fileType[]);
	virtual ~CFactory2dImage();

protected:
	static CFactory2dImage* s_pInstance;
	CFactory2dImage();

	
};

#endif //_CFACTORY2DIMAGE_H_