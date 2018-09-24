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

	I2dImage* Create2dImage(Byte fileType[]);

private:
	static CFactory2dImage* s_pInstance;
	CFactory2dImage();
	virtual ~CFactory2dImage();
	
};

#endif //_CFACTORY2DIMAGE_H_