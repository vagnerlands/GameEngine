#ifndef _CFACTORY2DIMAGEWIN_H_
#define _CFACTORY2DIMAGEWIN_H_

#include "CFactory2dImage.h"

using namespace Types;
using namespace std;

class CFactory2dImageWin : public CFactory2dImage
{
public:
	
	static void Initialize();

	std::shared_ptr<I2dImage> Create2dImage(Byte fileType[]) override;

private:
	CFactory2dImageWin();
	virtual ~CFactory2dImageWin();
	
};

#endif //_CFACTORY2DIMAGEWIN_H_