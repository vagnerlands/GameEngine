#include "JobList.h"

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
	JobList::JobList(): m_requests(), m_results()
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
			m_requests.push_back(id);
		}

		return true;
	}

	void JobList::PushResult(Graphics::IModel* result)
	{
		m_results.push_back(result);
	}

	void _Utils::JobList::Retrieve(Job& out)
	{
		out = m_requests.front();
	}

	Graphics::IModel* JobList::GetNextResult()
	{
		Graphics::IModel* pRet = nullptr;
		if (!m_results.empty())
		{
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