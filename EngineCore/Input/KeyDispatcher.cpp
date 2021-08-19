#include "KeyDispatcher.h"
#include "CGameController.h"

namespace _Keys
{
    KeyDispatcher* KeyDispatcher::pInstance = nullptr;

    KeyDispatcher& KeyDispatcher::Instance()
    {
        return *pInstance;
    }

    void KeyDispatcher::Event(Float dt, UByte key)
    {
        for (auto entry = m_listeners.begin(); entry != m_listeners.end(); entry++)
        {
            if ((*entry).first == key)
                (*entry).second(dt, mpGameController->m_bKey[key].cycles);
        }
    }

    void KeyDispatcherFactory::Create(CGameController* pGameController)
    {
        KeyDispatcher::pInstance = new KeyDispatcherFactory(pGameController);
    }

}