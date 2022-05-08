#ifndef CSOUNDHOLDER_H_
#define CSOUNDHOLDER_H_

#include "CommonTypes.h"
#include "IMutex.h"
#include "CResHandle.h"

using namespace Types;
using namespace std;

enum eSoundType
{
	eSoundType_Music = 0,
	eSoundType_Effect
};

class CSoundHolder 
{
public:
	//typedef unordered_map<string, Types::Byte*> SoundContentMap;
	static bool Create(const string& pathToSounds, UInt32 maxAllocSize);
	void PlaySoundById(const string& id, eSoundType type, Float volume);
	void RemoveSound(const string& id);
	void Destroy(const string& id);
	~CSoundHolder();

	//void AudioCallback(void* userData, Uint8* stream, int streamLength);
	void LockMix();
	void UnlockMix();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(const string& removeItem);

	// local instance
	static CSoundHolder* s_pInstance;

	static void PlaySound();


private:

	struct SoundAttributes
	{
		SoundAttributes() : m_id(""), m_type(eSoundType_Music), m_volume(1.f) {}
		explicit SoundAttributes(const string& id, eSoundType type, Float volume) : m_id(id), m_type(type), m_volume(volume){}
		string m_id;
		eSoundType m_type;
		Float m_volume;
	};


	bool loadSound(const string& id);

	void getNextSound(SoundAttributes& out);

	void finish();

	void initialize();

	CSoundHolder(const string& pathToSounds, UInt32 maxAllocSize);

	// local hashmap for textures to be generated
	//SoundContentMap m_soundMap;

	list<SoundAttributes> m_soundsToPlay;

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