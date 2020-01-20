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
    vector<Types::SModelTexture> textures_loaded;
	// meshes contain also list of textures
    vector<Types::SModelMesh> meshes;
    string directory;
	string m_shaderName;
    bool gammaCorrection;

    /*  Functions   */
    Model() : gammaCorrection(false)
    {
		m_shaderName = "model";
    }

	void Load(string const& path, bool gamma = false)
	{
		gammaCorrection = gamma;
		loadModel(path);
	}

	const string& GetShaderSuggestion() const
	{
		return m_shaderName;
	}

    
private:
    // Functions

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(UInt32 i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(UInt32 i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    SModelMesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        SModelMesh processedMesh;

        // Walk through each of the mesh's vertices
        for(UInt32 i = 0; i < mesh->mNumVertices; i++)
        {
            SModelVertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
			if (mesh->mNormals) // does the mesh contain normal coordinates
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			else
			{
				vertex.Normal = glm::vec3(0.f, 1.f, 0.f);
			}
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            // tangent
            if (mesh->mTangents != nullptr)
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            else
            {
                vertex.Tangent = glm::vec3(0.f, 0.f, 0.f);
            }
            // bitangent
            if (mesh->mBitangents != nullptr)
            {
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.Bitangent = glm::vec3(0.f, 0.f, 0.f);
            }
            processedMesh.m_vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(UInt32 i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(UInt32 j = 0; j < face.mNumIndices; j++)
                processedMesh.m_indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<SModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuseMap");
        processedMesh.m_textures.insert(processedMesh.m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<SModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specularMap");
        processedMesh.m_textures.insert(processedMesh.m_textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<SModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normalMap");
        processedMesh.m_textures.insert(processedMesh.m_textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<SModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        processedMesh.m_textures.insert(processedMesh.m_textures.end(), heightMaps.begin(), heightMaps.end());

		// TODO: make this look nicer - quick and dirty implementation
		if ((diffuseMaps.size() > 0) 
			&& (specularMaps.size()) 
			&& (normalMaps.size()))
		{
			m_shaderName = "model_dns";
		}
		else if ((diffuseMaps.size() > 0)
			&& (specularMaps.size() > 0))
		{
			m_shaderName = "model_ds";
		}
		else if ((diffuseMaps.size() > 0)
			&& (normalMaps.size() > 0))
		{
			m_shaderName = "model_dn";
		}
        
        // return a mesh object created from the extracted mesh data
        return processedMesh;
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<SModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string& typeName)
    {
        vector<SModelTexture> textures;
        for(UInt32 i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(UInt32 j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].m_filename.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    skip = true; 
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                SModelTexture texture;
                texture.m_uniformName = typeName;
                texture.m_filename = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};

#endif
