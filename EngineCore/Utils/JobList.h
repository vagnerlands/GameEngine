#ifndef _JOBLIST_H_
#define _JOBLIST_H_

#include "CommonTypes.h"
#include <list>

using namespace Types;
namespace Graphics { class IModel; }

namespace _Utils
{
	//! this data structure holds a unique member
	//! another member with same operator()== will not be added to the list
	//! underlying, it holds a list data structure
	class JobList
	{
		friend class Job;
	private:
		std::list<std::string> m_requests;
		std::list<Graphics::IModel*> m_results;
		void Pop(const std::string& id);
	public:
		JobList();
		bool PushRequest(const std::string& id);
		void PushResult(Graphics::IModel* pModel);
		void Retrieve(class Job& out);
		Graphics::IModel* GetNextResult();
		bool IsEmpty() const;

	};

	class Job
	{
	public:
		explicit Job(JobList& jobList);
		~Job();
		const std::string& Get() const;
		void operator=(std::string& rhs);
		
	private:
		std::string* m_id;
		JobList& m_jobs;
	};

}
#endif //_JOBLIST_H_
