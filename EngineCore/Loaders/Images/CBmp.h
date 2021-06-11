#ifndef _CBMP_H_
#define _CBMP_H_

#include "I2dImage.h"

using namespace Types;
using namespace std;


class CBmp : public I2dImage
{
public:
	CBmp();

    CBmp(CBmp&& rhs) : I2dImage(std::move(rhs))
    {}

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~CBmp();
	
};

#endif //_CBMP_H_