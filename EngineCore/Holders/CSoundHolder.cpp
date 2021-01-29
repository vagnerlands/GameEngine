#include "CSoundHolder.h"
#include "MutexFactory.h"
#include "sdl/SDL.h"

#include "IThread.h"
#include "ISemaphore.h"
#include "ThreadFactory.h"
#include "SemaphoreFactory.h"

#include <time.h>
#include <iostream>

using namespace std;

struct AudioData
{
	Uint8* position;
	Uint32 length;
};

void AudioCallback(void* userData, Uint8* stream, int streamLength)
{
	CSoundHolder::s_pInstance->LockMix();
	AudioData* audio = (AudioData*)userData;
	Uint32 length = (Uint32)streamLength;
	if (audio->length == 0) 
		return;

	if (length > audio->length) {
		length = audio->length;
	}

	SDL_memset(stream, 0, length);
	//SDL_MixAudioFormat(stream, audio->position, SDL_AUDIO_ALLOW_FORMAT_CHANGE, length, 100);
	//SDL_memcpy(stream, audio->position, length);
	SDL_MixAudio(stream, audio->position, length, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	//memcpy(stream, audio->position, length);

	audio->position += length;
	audio->length -= length;

	//cout << "writing " << length << " bytes, from ";
	//cout << (int)audio->position << " to " << (int)stream << endl;

	CSoundHolder::s_pInstance->UnlockMix();
}

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
		SDL_Init(SDL_INIT_AUDIO);
	}
	return false;
}

void CSoundHolder::PlaySoundById(const string& id)
{
	// 
	//bool isLoaded = true;
	// then try to find it in the textures map
	//SoundContentMap::iterator result = m_soundMap.find(id);
	//if (result == m_soundMap.end())
	//{
	//	//isLoaded = loadSound(id);
	//}

	m_soundMapMutex->mutexLock();
	m_soundsToPlay.push_back(id);
	m_soundMapMutex->mutexUnlock();
	//IThread* pThr = ThreadFactory::Instance().Create(string("TSoundPlayer_" + id).data(), 0x02, CSoundHolder::PlaySound);
	// allow the sound thread to start working
	m_pSemaphore->signal();

	

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
	while (1) 
	{
		// set this thread to rest
		CSoundHolder::s_pInstance->finish();

		string id;
		CSoundHolder::s_pInstance->getNextSound(id);

		SDL_AudioSpec wavSpec;
		Uint8* wavStart;
		Uint32 wavLength;
		std::string filePath = "./Assets/" + id;

		if (SDL_LoadWAV(filePath.data(), &wavSpec, &wavStart, &wavLength) == NULL)
		{
			printf("Error: file could not be loaded as an audio file.\n");
		}

		AudioData audio;
		audio.position = wavStart;
		audio.length = wavLength;

		wavSpec.callback = AudioCallback;
		wavSpec.userdata = &audio;

		/* Open the audio device */
		if (SDL_OpenAudio(&wavSpec, NULL) < 0) {
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			exit(-1);
		}

		/* Start playing */
		SDL_PauseAudio(0);

		while (audio.length > 0)
		{
			SDL_Delay(100);
		}

		SDL_CloseAudio();
		SDL_FreeWAV(wavStart);

	}
}

bool
CSoundHolder::loadSound(const string& id)
{
	//// assumes it passed
	//bool ret = true;
	//// start loading measuring time
	//clock_t start = clock();

	//// cache missed - must reload it from resources db
	//CResource resourceItem(id);

	//Byte*  dataStream = m_pResHandler->VAllocateAndGetResource(resourceItem);
	//UInt32 dataLength = m_pResHandler->VGetResourceSize(resourceItem);
	//// status OK
	//if (dataStream != 0)
	//{
	//	m_soundMap.insert({ id, dataStream });
	//}
	//else
	//{
	//	ret = false;
	//	DEBUG_OUT("Sound %s not found\n", id);
	//}
	//
	//// time measurement
	//printf(" loading tex [%s] %.2fms\n", id.data(), (float)(clock() - start));

	//return ret;

	return true;
}

void CSoundHolder::getNextSound(string& out)
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
	// create semaphore
	m_pSemaphore = SemaphoreFactory::Instance().Create("TSoundSem", 1);
	// create a thread
	m_pThread = ThreadFactory::Instance().Create("TSoundPlayer", 0x02, CSoundHolder::PlaySound);
}


void CSoundHolder::RemoveSound(const string& id)
{
	//SoundContentMap::iterator it = m_soundMap.find(id);
	//if (it == m_soundMap.end())
	//{
	//	// this shouldn't happen - never, but if happens, trying 
	//	// to erase will cause an exception - so must quit method
	//	return;
	//}
	//delete[] it->second;
	//m_soundMap.erase(it);
}


CSoundHolder::~CSoundHolder()
{
/*	while (!m_soundMap.empty()) 
	{		
		SoundContentMap::iterator it = m_soundMap.begin();
		delete[] it->second;
		m_soundMap.erase(it);
	}*/		

	m_pSemaphore->destroy();
	m_pThread->destroy();

	SDL_Quit();
}

