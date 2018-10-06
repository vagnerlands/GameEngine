#include "CModelHolder.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>

#include <objParser/objLoader.h>
#ifdef WIN32
#include "CWinMutex.h"
#include "CModelOGL.h"
#endif

CModelHolder* CModelHolder::s_pInstance = NULL;

CModelHolder::~CModelHolder()
{
	while (!m_mapModels.empty())
	{
		if (m_mapModels.begin()->second != NULL)
			delete m_mapModels.begin()->second;
		m_mapModels.erase(m_mapModels.begin());
	}	
}

bool CModelHolder::Create(const string pathToModelFile)
{
	if (s_pInstance == nullptr)
	{
		// creates instance of CModelHolder
		s_pInstance = new CModelHolder(pathToModelFile);
		// tries to open the resource file - out of the constructor so errors may be reported
		if ((s_pInstance != nullptr) && (s_pInstance->m_modelFiles->VOpen()))
		{
			return true;
		}
	}
	return false;
}

void 
CModelHolder::OnRemoveEvent(string removeItem)
{
	s_pInstance->RemoveModel(removeItem);
}

CModelHolder::CModelHolder(std::string pathToResources)
	: m_modelFiles(new CResourceZipFile(pathToResources.data(), this->OnRemoveEvent))
{
#ifdef WIN32
	m_pModelContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
	if (m_pModelContentMapMutex != NULL)
	{
		m_pModelContentMapMutex->createMutex("ModelContentMap");
	}
}

void 
CModelHolder::LoadModel(const string modelId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resModel(modelId);
	Byte* modelStream = m_modelFiles->VAllocateAndGetResource(resModel);

	// checks if return value isn't null
	if (modelStream != 0)
	{
		// material stream 
		string materialId = modelId.substr(0, modelId.find_last_of('.'));
		materialId += ".mtl";
		CResource resMaterial(materialId);
		// tries to get the material data
		Byte* materialStream = m_modelFiles->VAllocateAndGetResource(resMaterial);
		// finally, perform allocation
		AddModelContent(modelId, modelStream, materialStream);

		// release resources
		if (materialStream != 0)
		{
			delete[] materialStream;
		}
		delete[] modelStream;
	}
	else
	{
		DEBUG_OUT("Model %s not found in resource file!\n", modelId);
	}	

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));
}

void
CModelHolder::AddModelContent(string modelId, Byte* bytesStream, Byte* materialStream)
{
	// new node to be allocated
	SModelData newNode;
	// will be deallocated at the end of this function
	shared_ptr<objLoader> objData (new objLoader());
	// parses the loaded model
	objData->load(bytesStream, materialStream, true);

	printf("Number of vertices: %i\n", objData->vertexCount);
	printf("Number of vertex normals: %i\n", objData->normalCount);
	printf("Number of texture coordinates: %i\n", objData->textureCount);
	printf("\n");

	for (int i = 0; i < objData->materialCount; i++)
	{
		SMaterialAttr newMaterial;
		obj_material* m = objData->materialList[i];

		newMaterial.m_ambient = IvVector3(m->amb[0], m->amb[1], m->amb[2]);
		newMaterial.m_specular = IvVector3(m->spec[0], m->spec[1], m->spec[2]);
		newMaterial.m_diffuse = IvVector3(m->diff[0], m->diff[1], m->diff[2]);
		newMaterial.m_glossy = m->glossy;
		newMaterial.m_reflect = m->reflect;
		newMaterial.m_refract = m->refract;
		newMaterial.m_shiny = m->shiny;
		newMaterial.m_trans = m->trans;

		newNode.m_material.push_back(newMaterial);
	}

	bool recalculateNormals = true;

	if (objData->normalCount > 0)
	{
		recalculateNormals = false;
		for (int i = 0; i < objData->normalCount; i++)
		{
			// temporary normal object
			obj_vector* n = objData->normalList[i];
			newNode.m_normals.push_back(IvVector3(n->e[0], n->e[1], n->e[2]));
		}
	}

	for (int i = 0; i < objData->vertexCount; i++)
	{
		// temporary vertex object
		obj_vector *o = objData->vertexList[i];

		newNode.m_vertices.push_back(IvVector3(o->e[0], o->e[1], o->e[2]));

		if (recalculateNormals)
		{
			//! if normals are not present - calculate them!
			// calculate the length of the axis of the vertex
			Float length = sqrt(pow(o->e[0], 2.0)
				+ pow(o->e[1], 2.0)
				+ pow(o->e[2], 2.0));

			// use calculate normals 
			// divide the vertex by the calculate length of the vertexes
			newNode.m_normals.push_back(IvVector3(o->e[0] / length, o->e[1] / length, o->e[2] / length));
		}
	}

	for (int i = 0; i < objData->textureCount; i++)
	{
		obj_vector* t = objData->textureList[i];
		newNode.m_textures.push_back(IvVector2(t->e[0], t->e[1]));
	}

	for (int i = 0; i < objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];

		for (int j = 0; j < o->vertex_count; j++)
		{
			
			//newNode.m_normalsIndexed.push_back(o->normal_index[j]);
			newNode.m_indexes.push_back(o->vertex_index[j]);
			//if (o->material_index != -1)
			//{
			//	newNode.m_texturesIndexed.push_back(o->texture_index[j]);
			//}			
		}
	}



	/*m_pModelContentMapMutex->mutexLock();
	m_models.insert(make_pair(modelId, newNode));
	m_pModelContentMapMutex->mutexUnlock();*/
	Graphics::IModel* pModelObj = new Graphics::CModelOGL();

	pModelObj->Create(newNode);

	m_mapModels.insert(make_pair(modelId, pModelObj));
	// objData will lose its last reference and it will be deallocated
}

void 
CModelHolder::RemoveModel(const string textId)
{
	ModelObject::iterator it = m_mapModels.find(textId);
	if (it == m_mapModels.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	if (it->second != NULL)
	{
		delete it->second;
	}

	m_mapModels.erase(it);

	//_CrtDumpMemoryLeaks();
}

Graphics::IModel&
CModelHolder::GetModelById(const string modelId)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		return *result->second;
	}

	// cache miss - then add this texture to the process list
	LoadModel(modelId);
	
	// if it somehow failed, returns -1
	return *(Graphics::IModel*)(NULL);
}

void CModelHolder::DrawModelById(const string modelId)
{
	// then try to find it in the textures map
	ModelObject::iterator result = m_mapModels.find(modelId);
	if (result != m_mapModels.end())
	{
		result->second->Draw();
	}
	else
	{
		// cache miss - then add this texture to the process list
		LoadModel(modelId);
	}
}
