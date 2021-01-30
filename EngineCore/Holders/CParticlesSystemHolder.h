#ifndef CPARTICLESMANAGER_H_
#define CPARTICLESMANAGER_H_

#include "CommonTypes.h"
#include "CResHandle.h"
#include "IvVector3.h"
#include <unordered_map>
#include <memory>
#include "CParticle.h"

using namespace Types;
using namespace std;
namespace Graphics
{
	class CParticle;
	class IDrawable;
}

class IMutex;

class CParticlesSystemHolder
{
public:

	//typedef unordered_map<string, SModelData> ModelMap;
	typedef unordered_map<string, Graphics::CParticle*> DisplayObject;

	// always create before using
	static bool Create();
	virtual ~CParticlesSystemHolder();

	bool Create(const string& pathToModelFile);
    
	void RemoveParticle(const string& particleId);

	Graphics::IDrawable* GetParticleById(const string& particleId);

	void Update(float dt);

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(const string& removeItem);
	// local instance
	static CParticlesSystemHolder* s_pInstance;

private:
	CParticlesSystemHolder(const string& pathToResources);

	Graphics::CParticle* findParticleInDB(const std::string& id);

	// hash map for the raw data models (vertexes, normals, material, etc)
	//ModelMap m_models;
	// hash map for the IModels* - containing what you need to render this model
	DisplayObject m_mapParticles;
	// mutex for m_processes
	IMutex* m_pParticlesContentMapMutex;


};

#endif //CPARTICLESMANAGER_H_