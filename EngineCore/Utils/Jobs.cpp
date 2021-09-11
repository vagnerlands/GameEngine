#include "Jobs.h"
#include "MutexFactory.h"
#include "IMutex.h"

_Utils::Jobs::Jobs() : m_pRequestsMutex(MutexFactory::Instance().Create("jobs"))
{
}

_Utils::Jobs & _Utils::Jobs::Instance()
{
    static _Utils::Jobs instance;
    return instance;
}

void _Utils::Jobs::PushRequest(std::function<std::shared_ptr<I2dImage>(string&)> func, string arg)
{
    LockGuard t(m_pRequestsMutex);
    
    bool found = false;
    // only add to the job list if it doesn't exist in the job list
    for (auto i : m_requests)
    {
        if (i.second == arg)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_requests.push_back({ func, arg });
    }
}

std::pair<std::shared_ptr<I2dImage>, string> _Utils::Jobs::GetNextResult()
{
    LockGuard t(m_pRequestsMutex);
    if (!m_results.empty())
    {
        auto res = m_results.back();
        m_results.pop_back();
        return res;
    }
    return {nullptr, ""};
}

void _Utils::Jobs::Process()
{
    while (!m_requests.empty())
    {
        LockGuard t(m_pRequestsMutex);
        auto& next = m_requests.back();
        
        std::shared_ptr<I2dImage> res = next.first(next.second); // process the loading                
        m_results.push_back({ res, next.second }); // store the results (graphics commands must be done in the main thread)

        m_requests.pop_back();
    }
}
