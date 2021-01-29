#ifndef CSOUNDHOLDER_H_
#define CSOUNDHOLDER_H_

#include "CommonTypes.h"
#include "IMutex.h"
#include "CResHandle.h"

using namespace Types;
using namespace std;

class CSoundHolder 
{
public:
	//typedef unordered_map<string, Types::Byte*> SoundContentMap;
	static bool Create(const string& pathToSounds, UInt32 maxAllocSize);
	void PlaySoundById(const string& id);
	void RemoveSound(const string& id);
	void Destroy(const string& id);
	CSoundHolder::~CSoundHolder();

	//void AudioCallback(void* userData, Uint8* stream, int streamLength);
	void LockMix();
	void UnlockMix();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(const string& removeItem);

	// local instance
	static CSoundHolder* s_pInstance;

	static void PlaySound();


private:
	bool loadSound(const string& id);

	void getNextSound(string& out);

	void finish();

	void initialize();

	CSoundHolder(const string& pathToSounds, UInt32 maxAllocSize);

	// local hashmap for textures to be generated
	//SoundContentMap m_soundMap;

	list<string> m_soundsToPlay;

	// mutex for m_processes
	IMutex* m_soundMapMutex;

	IResourceFile*          m_pResHandler;
    // reserved size for VRAM
    UInt32                  m_maxAllocSize;
    // total VRAM size in usage 
    UInt32                  m_sizeInUse;
	// thread to run control the audio flow
	class IThread*			m_pThread;
	class ISemaphore*		m_pSemaphore;
    

};

#endif //CSOUNDHOLDER_H_