#include "CResHandle.h"

CResHandle*
CResource::VCreateHandle(const char* buffer, unsigned int size, CResCache* pResCache) {
	CResHandle* retVal = new CResHandle(*this, (char*)buffer, size, pResCache);
	return retVal;
}

void 
CResourceZipFile::VOnRemoveEvent(string removedItem)
{
	m_textMngHandle(removedItem);
}

CResourceZipFile::~CResourceZipFile()
{
	delete m_pZipFile;
}

bool CResourceZipFile::VOpen()
{
	m_pZipFile = new ZipFile();
	if (m_pZipFile)
	{
		_isInitialized = m_pZipFile->Init(m_resFileName.c_str());
	}
	return _isInitialized;
}

int CResourceZipFile::VGetResourceSize(const CResource &r)
{
	int size = 0;
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == -999999)
		throw std::runtime_error("Entry '" + r.m_name + "' was not found in '" + m_resFileName + "'");
	size = m_pZipFile->GetFileLen(resourceNum);
	return size;
}

int
CResourceZipFile::VGetResource(const CResource &r, char *buffer)
{
	int size = 0;
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum != -999999)
	{
		size = m_pZipFile->GetFileLen(resourceNum);
		m_pZipFile->ReadFile(resourceNum, buffer);
	}
	return 0;
}

char* 
CResourceZipFile::VAllocateAndGetResource(const CResource &r)
{
	if (!_isInitialized)
		throw std::runtime_error("Resource Zip was not initialized properly " + m_resFileName);
	// data to be returned to user - starts with no data (null)
	char* contentBuffer = 0;
	// retrieve file size
	const Int32 fileSizeInBytes = VGetResourceSize(r);
	// if the file was found, then we must allocate its data
	if (fileSizeInBytes > 0)
	{
		// allocate this data - make sure we'll delete it after usage!
		contentBuffer = new char[fileSizeInBytes];
		// checks if allocation was succesfull - maybe the system ran out of memory
		if (contentBuffer != 0)
		{
			// method return value is useless, so it's not being checked
			VGetResource(r, contentBuffer);
		}
		else
		{
			DEBUG_OUT("Not enought memory to allocate resource %s\n", r.m_name.data());
		}
	}

	return contentBuffer;
}

CResHandle::CResHandle(CResource& resource, char* buffer, unsigned int size, CResCache* pResCache)
	: m_resource(resource) {
	m_buffer = buffer;
	m_size = size;
	m_pResCache = pResCache;
}

CResHandle::~CResHandle() 
{
	if (m_buffer)
		delete[] m_buffer;
	m_pResCache->MemoryHasBeenFreed(m_size);
}

CResCache::~CResCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}

	// deletes m_file - call proper function
}

bool
CResCache::Init()
{
	return m_file->VOpen();
}


shared_ptr<CResHandle>
CResCache::GetHandle(CResource* r)
{
	shared_ptr<CResHandle> handle(Find(r));
	if (handle == NULL)
	{
		handle = Load(r);
	}
	else
	{
		Update(handle);
	}
	return handle;
}

void
CResCache::Flush()
{
	while (!m_lru.empty())
	{
		shared_ptr<CResHandle> handle = *(m_lru.begin());
		Free(handle);
		m_lru.pop_front();
	}
}


shared_ptr<CResHandle>
CResCache::Find(CResource* r)
{
	unordered_map<string, shared_ptr<CResHandle>>::iterator retVal = m_resourcesMap.find(r->m_name);
	if (retVal != m_resourcesMap.end())
		return retVal->second;
	else
		return NULL;
}

shared_ptr<CResHandle>
CResCache::Load(CResource* r)
{
	int size = m_file->VGetResourceSize(*r);
	char* buffer = Allocate(size);

	if (buffer == NULL) {
		// CResCache ran out of memory
		return shared_ptr<CResHandle>();
	}

	shared_ptr<CResHandle> handle(r->VCreateHandle(buffer, size, this));
	handle->VLoad(m_file);

	m_lru.push_front(handle);
	m_resourcesMap[r->m_name] = handle;

	return handle;
}

void
CResCache::Free(shared_ptr<CResHandle> gonner)
{
	list<shared_ptr<CResHandle>>::iterator iter = m_lru.begin();
	while (*iter != gonner) iter++;

	// callback event when a resource is deallocated
	// tells the texture manager that this resource is gonne
	m_file->VOnRemoveEvent(gonner->GetResource().m_name);

	// the handle is already allocated
	if (*iter == gonner) {
		// removes this handle from the least recently used
		m_lru.remove(gonner);
	}
}

bool
CResCache::MakeRoom(unsigned int size)
{
	if (size > m_cacheSize)
	{
		return false;
	}

	while (size > (m_cacheSize - m_allocated))
	{
		if (m_lru.empty())
			return false;

		FreeOneResource();
	}

	return true;
}

char*
CResCache::Allocate(unsigned int size)
{
	if (!MakeRoom(size))
	{
		return NULL;
	}

	char* mem = new char[size];
	if (mem)
		m_allocated += size;

	return mem;
}

void
CResCache::FreeOneResource()
{
	list< shared_ptr<CResHandle> >::iterator iter = m_lru.end();
	iter--;

	shared_ptr<CResHandle> handle = *iter;

	// callback event when a resource is deallocated
	// tells the texture manager that this resource is gonne
	m_file->VOnRemoveEvent(handle->GetResource().m_name);

	m_lru.pop_back();
	m_resourcesMap.erase(handle->GetResource().m_name);
}

void
CResCache::MemoryHasBeenFreed(unsigned int size)
{
	m_allocated -= size;
}
