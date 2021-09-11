#ifndef _JOBS_H_
#define _JOBS_H_

#include "CommonTypes.h"
#include <list>
#include <functional>
#include <string>
#include "CTextureHolder.h"

using namespace Types;
using namespace std;
namespace Graphics
{
    class ITexture;
}
class IMutex;

namespace _Utils
{
	//! this data structure holds a unique member
	//! another member with same operator()== will not be added to the list
	//! underlying, it holds a list data structure
	class Jobs
	{
		//friend class Jab;
        IMutex* m_pRequestsMutex;
		std::list<std::pair< std::function<std::shared_ptr<I2dImage>(string&)>, string >> m_requests;
        std::list< std::pair< std::shared_ptr<I2dImage>, string >> m_results;
	public:
        Jobs();
        static Jobs& Instance();
		void PushRequest(std::function<std::shared_ptr<I2dImage>(string&)>, string arg);
        std::pair< std::shared_ptr<I2dImage>, string > GetNextResult();
		void Process();
	};

}
#endif //_JOBS_H_
