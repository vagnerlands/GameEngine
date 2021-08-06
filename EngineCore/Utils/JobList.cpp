#include "JobList.h"
#include "IMutex.h"
#include "MutexFactory.h"

namespace _Utils
{
	void JobList::Pop(const std::string& id)
	{
		m_requests.remove(id);
	}
	bool JobList::IsEmpty() const
	{
		return m_requests.empty();
	}
    JobList::JobList() :
        m_pRequestsMutex(MutexFactory::Instance().Create("JobRequestsMutex")),
        m_requests(),
        m_pResultsMutex(MutexFactory::Instance().Create("JobResultsMutex")),
        m_results()
	{
	}
	bool JobList::PushRequest(const std::string& id)
	{
		bool found = false;
		for (auto it = m_requests.begin(); it != m_requests.end(); it++)
		{
			if (id == (*it))
			{
				found = true;
			}
		}

		if (!found)
		{
            LockGuard mutex(m_pRequestsMutex);
			m_requests.push_back(id);
		}

		return true;
	}

	void JobList::PushResult(shared_ptr<Graphics::IModel> result)
	{
        LockGuard mutex(m_pResultsMutex);
		m_results.push_back(result);
	}

	void _Utils::JobList::Retrieve(Job& out)
	{
        LockGuard mutex(m_pRequestsMutex);
		out = m_requests.front();
	}

    shared_ptr<Graphics::IModel> JobList::GetNextResult()
	{		
        shared_ptr<Graphics::IModel> pRet = nullptr;
		if (!m_results.empty())
		{
            LockGuard mutex(m_pResultsMutex);
            pRet = m_results.front();
			m_results.pop_front();
		}

		return pRet;
	}

	Job::Job(JobList& jobList) : m_id(nullptr), m_jobs(jobList)
	{
	}

	Job::~Job()
	{
		m_jobs.Pop(*m_id);
	}

	const std::string& Job::Get() const
	{
		return *m_id;
	}

	void Job::operator=(std::string& rhs)
	{
		m_id = &rhs;
	}
}