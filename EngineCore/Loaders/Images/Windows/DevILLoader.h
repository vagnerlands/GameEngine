#ifndef _DevILLoader_H_
#define _DevILLoader_H_

#include "I2dImage.h"

using namespace Types;
using namespace std;


class DevILLoader : public I2dImage
{
public:
	DevILLoader();

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~DevILLoader();

private:
		
};

#endif //_DevILLoader_H_