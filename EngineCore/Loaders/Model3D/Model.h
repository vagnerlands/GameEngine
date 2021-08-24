#ifndef MODEL_H
#define MODEL_H

#include "OGLTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
using namespace Types;

class Model 
{
public:
    
    // Keep track of already loaded textures in order to not load the same texture twice in the VRAM
    vector<Types::SModelTexture>    textures_loaded;
	// meshes contain also list of textures
    vector<Types::SModelMesh>       meshes;
    bool                            gammaCorrection;
    bool                            hasAnimations;
    Assimp::Importer*               m_importer;
    SBoneInformation*               m_pBoneInformation;
    AABB                            m_boundaryBox;
    
    // public methods
    Model();
    /*  Functions   */
    Model(Assimp::Importer& importer);
    ~Model(); 

    void Load(string const& path, bool gamma = false);
    void SetBoneInformation(SBoneInformation* pBoneInf);
    
private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void                    loadModel(string const &path);
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void                    processNode(aiNode *node, const aiScene *scene);
    SModelMesh              processMesh(aiMesh *mesh, const aiScene *scene);
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<SModelTexture>   loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string& typeName);
    AABB                    calculateBoundingBox(const vector<SModelMesh>& meshes) const;
    UInt32                  estimateMeshesCount(aiNode * node) const;
    void                    estimateMeshesCountLooper(aiNode * node, UInt32& meshesCount) const;    
};

inline void Model::Load(string const & path, bool gamma)
{
    gammaCorrection = gamma;
    loadModel(path);
}

inline void Model::SetBoneInformation(SBoneInformation * pBoneInf)
{
    m_pBoneInformation = pBoneInf;
}

#endif
