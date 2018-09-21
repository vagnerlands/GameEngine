#ifndef _IRESOURCEFILE_H_
#define _IRESOURCEFILE_H_
//#include "CResource.h"
#include "CommonTypes.h"
using namespace Types;
class IResourceFile {
	friend class CResource;
public:
	virtual bool VOpen() = 0;
	virtual int VGetResourceSize(const CResource &r) = 0;
	virtual int VGetResource(const CResource &r, char* buffer) = 0;
	virtual char* VAllocateAndGetResource(const CResource &r) = 0;
	virtual void VOnRemoveEvent(string removedItem) = 0;
protected: 
	OnRemoveEvent m_textMngHandle;

};

#endif 
