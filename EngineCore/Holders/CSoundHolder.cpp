#include "CSoundHolder.h"
#include "MutexFactory.h"

#include "IThread.h"
#include "ISemaphore.h"
#include "ThreadFactory.h"
#include "SemaphoreFactory.h"

#include <time.h>
#include <iostream>

#include "Sound/audio.h"

using namespace std;

CSoundHolder* CSoundHolder::s_pInstance = NULL;

bool CSoundHolder::Create(const string& pathToTexturesFile, UInt32 maxAllocSize)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CSoundHolder(pathToTexturesFile, maxAllocSize);
		// tries to open the resource file - out of the constructor so errors may be reported
		if ((s_pInstance != nullptr) 
            && (s_pInstance->m_pResHandler->VOpen()))
		{
			return true;
		}
		s_pInstance->initialize();
		/* Init Simple-SDL2-Audio */
		initAudio();

		/* While using delay for showcase, don't actually do this in your project */
		SDL_Delay(5000);
	}
	return false;
}

void CSoundHolder::PlaySoundById(const string& id, eSoundType type, Float volume)
{
	m_soundMapMutex->mutexLock();
	m_soundsToPlay.push_back(SoundAttributes(id, type, volume));
	m_soundMapMutex->mutexUnlock();
	// thread will run immediatelly
	IThread* pThr = ThreadFactory::Instance().Create(string("thTSoundPlayer_" + id).data(), 0x02, CSoundHolder::PlaySound);
}

CSoundHolder::CSoundHolder(const string& pathToTexturesFile, UInt32 maxAllocSize) :
    m_pResHandler(new CResourceZipFile(pathToTexturesFile.data(), this->OnRemoveEvent)),
    m_maxAllocSize(maxAllocSize),
    m_sizeInUse(0U)
{

}

void CSoundHolder::LockMix()
{
	m_soundMapMutex->mutexLock();
}

void CSoundHolder::UnlockMix()
{
	m_soundMapMutex->mutexUnlock();
}

void
CSoundHolder::OnRemoveEvent(const string& removeItem)
{
	s_pInstance->RemoveSound(removeItem);
}

void CSoundHolder::PlaySound()
{ 
	SoundAttributes sound;
	// extracts and pop from the queue - it uses mutex internally, so no need to protect this critical section
	CSoundHolder::s_pInstance->getNextSound(sound);
	// music will play in loop, while sound effect will play once.
	// only 1x music can play at a time, while up to 25 different sound effects may play at the same time...
	switch (sound.m_type)
	{
		case eSoundType_Music:
			playMusic(string("./Assets/" + sound.m_id).data(), static_cast<Int32>(SDL_MIX_MAXVOLUME * sound.m_volume));
			break;
		case eSoundType_Effect:
			playSound(string("./Assets/" + sound.m_id).data(), static_cast<Int32>(SDL_MIX_MAXVOLUME * sound.m_volume));
			break;
	}
	
	SDL_Delay(1000);
}

bool
CSoundHolder::loadSound(const string& id)
{
	// might be useful later
	return true;
}

void CSoundHolder::getNextSound(SoundAttributes& out)
{
	// lock mutex
	m_soundMapMutex->mutexLock();
	// makes a copy of the last item
	out = m_soundsToPlay.back();
	// remove it from the list
	m_soundsToPlay.pop_back();
	// release mutex
	m_soundMapMutex->mutexUnlock();
}

void CSoundHolder::finish()
{
	// return this thread back to sleep
	m_pSemaphore->wait();
}

void CSoundHolder::initialize()
{
	// cross platform implementation for mutex creation
	m_soundMapMutex = MutexFactory::Instance().Create("SoundMapMutex");
}


void CSoundHolder::RemoveSound(const string& id)
{

}


CSoundHolder::~CSoundHolder()
{
/*	while (!m_soundMap.empty()) 
	{		
		SoundContentMap::iterator it = m_soundMap.begin();
		delete[] it->second;
		m_soundMap.erase(it);
	}*/		

	//m_pSemaphore->destroy();
	//m_pThread->destroy();

	SDL_Quit();
}

