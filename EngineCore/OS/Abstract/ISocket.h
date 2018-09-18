#ifndef ISOCKET_H_
#define ISOCKET_H_

#include "CommonTypes.h"

using namespace std;
using namespace Types;

class CICommand;

class ISocket 
{
public:
	virtual ~ISocket() {

	}
	// Returns the status of a socket connection
	// 0 succeeded;
	// any other value is fail
	virtual ESocketConnectionStatus startListenSocket() = 0;

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// content of buffer till it's size will be copied
	virtual Int32 write(const Byte* buffer, Int32 const size) = 0;

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// the buffer will be copied to Byte* pointer
	virtual Int32 read(Byte* buffer, Int32* size) = 0;

	virtual  void closeSocket() = 0;

	virtual void invalidateSocket() = 0;

	virtual UInt16 getClientPort() = 0;
	virtual Byte* getClientIP() = 0;

	// set socket to recyclation
	void recycle()
	{
		m_recycle = true;
	}
	// get recyclation status from socket
	bool recycleStatus()
	{
		return m_recycle;
	}

protected:
	// shall be removed from DS
	bool m_recycle;
};

#endif
