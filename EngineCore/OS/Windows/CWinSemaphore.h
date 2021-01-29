#ifndef _CWinSemaphore_H_
#define _CWinSemaphore_H_

#include <Windows.h>
#include "CommonTypes.h"
#include "ISemaphore.h"

using namespace Types;
using namespace std;

class CWinSemaphore : public ISemaphore
{
public:
	CWinSemaphore();
	~CWinSemaphore();
	void createSemaphore(string thName, Int32 count) override;
	bool signal() override;
	bool wait() override;
	void destroy() override;
private:
	HANDLE m_semaphoreHandle;
};

#endif
