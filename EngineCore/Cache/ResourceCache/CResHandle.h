#ifndef _CRESHANDLE_H_
#define _CRESHANDLE_H_

#include "ZipFile.h"
#include "IResourceFile.h"

#include <memory>
#include <string>
#include <list>
#include <unordered_map>

using namespace std; 

class CResHandle;
class CResCache;

class CResource 
{
	
public:
	string m_name;
	CResource() 
	{
		m_name = "";
	}

	CResource(string name) {
		m_name = name;
	}

	void SetName(string name)
	{
		m_name = name;
	}

	virtual CResHandle* VCreateHandle(const char* buffer, unsigned int size, CResCache* pResCache);

};
// forward reclaration of ZipFile
class ZipFile;

class CResourceZipFile : public IResourceFile
{
	ZipFile* m_pZipFile;
	string m_resFileName;
	bool _isInitialized;

public:
	CResourceZipFile(const char* resFileName, void* textMngHandle) : 
		m_pZipFile(nullptr), 
		m_resFileName(""), 
		_isInitialized(false) 
	{
		m_pZipFile = NULL;
		m_resFileName = resFileName;
		m_textMngHandle = (OnRemoveEvent)textMngHandle;
	}
	virtual ~CResourceZipFile();

	virtual bool VOpen();
	virtual int VGetResourceSize(const CResource &r);
	virtual int VGetResource(const CResource &r, char *buffer);
	virtual char* VAllocateAndGetResource(const CResource &r);
	virtual void VOnRemoveEvent(string removedItem);
};

class CResHandle 
{
    // have to access the private objects of CResCache - for simplicity of design 
	friend class CResCache;
public:

	CResHandle(CResource& resource, char* buffer, unsigned int size, CResCache* pResCache);

	virtual ~CResHandle();

	virtual int VLoad(IResourceFile* file) {
		return file->VGetResource(m_resource, m_buffer);
	}

	virtual char* Buffer() {
		return m_buffer;
	}

	virtual CResource& GetResource() {
		return m_resource;
	}

	virtual unsigned int GetSize() {
		return m_size;
	}

protected:
	CResource m_resource;
	char* m_buffer;
	unsigned int m_size;
	CResCache* m_pResCache;
};



class CResCache 
{
	friend class CResHandle;
protected:

	list< shared_ptr<CResHandle> > m_lru;
	unordered_map< string, shared_ptr<CResHandle> > m_resourcesMap;

	IResourceFile* m_file;
	unsigned int m_cacheSize;
	unsigned int m_allocated;

	shared_ptr<CResHandle> Find(CResource* r);

	const void* Update(shared_ptr<CResHandle> handle) {
		// removes a handle from the LRU if this exists
		Free(handle);
		// promotes the handle to the front of the queue
		m_lru.push_front(handle);

		return &handle;
	}

	shared_ptr<CResHandle> Load(CResource* r);

	void Free(shared_ptr<CResHandle> gonner);

	bool MakeRoom(unsigned int size);

	char* Allocate(unsigned int size);

	void FreeOneResource();

public:
	CResCache(const unsigned int sizeInMb, IResourceFile *resFile) {
		m_cacheSize = sizeInMb * 1024 * 1024;
		m_allocated = 0;
		m_file = resFile;
	}

	~CResCache();

	bool Init();

	shared_ptr<CResHandle> GetHandle(CResource* r);

	void Flush();

	void MemoryHasBeenFreed(unsigned int size);

};




#endif //_CRESHANDLE_H_
