#ifndef _OglHelper_H_
#define _OglHelper_H_

#include "CommonTypes.h"
#include "glsl.h"
#include "CTextureHolder.h"
#include "Ilumination.h"
#include <time.h> // put this somewhere else, create a lib for time
#include <WinBase.h> // put this somewhere else

using namespace Types;

namespace Graphics
{
	class OglHelper
	{
	public:

        static void Graphics::OglHelper::applyTextures(cwc::glShader* pShader, const vector<SModelTexture>& textures)
        {
            const size_t cTexturesCount = textures.size();
            if (cTexturesCount > 0)
            {
                // checks whether is a cubemap - in this case, there will be a texture id for the 6 cube faces
                if (textures[0].m_isCubeMap)
                {
                    // who cares, set the first one as the active one
                    glActiveTexture(GL_TEXTURE0);
                    string name = textures[0].m_uniformName;
                    // now set the sampler to the correct texture unit
                    pShader->setTexture(textures[0].m_uniformName.data(), CTextureHolder::s_pInstance->getTextureVector(textures));
                }
                else
                {
                    // texture indexer
                    for (UInt32 ti = 0; ti < cTexturesCount; ti++)
                    {
                        glActiveTexture(GL_TEXTURE0 + ti); // active proper texture unit before binding
                                                          // retrieve texture number (the N in diffuse_textureN)
                        string name = textures[ti].m_uniformName;
                        // now set the sampler to the correct texture unit
                        pShader->setTexture(textures[ti].m_uniformName.data(), CTextureHolder::s_pInstance->getTextureById(textures[ti].m_filename));
                        // activate the respective texture
                        glActiveTexture(GL_TEXTURE0 + ti);
                    }
                    // apply shadows mapping, today, all shaders may receive this Texture3D
                    glBindTexture(GL_TEXTURE_CUBE_MAP, Graphics::Ilumination::Instance().GetShadowTexture());
                }
            }
            else
            {
                // no textures mapped, so apply default texture
                pShader->setTexture("diffuseMap", CTextureHolder::s_pInstance->getTextureById("__no_valid_texture__.bmp"));
                glActiveTexture(GL_TEXTURE0);

            }
        }

#pragma optimize( "", off )
        static cwc::glShader* OglHelper::generateShader(const string& shaderName)
        {
            // return value
            bool status = true;
            // start loading measuring time
            clock_t start = clock();

            // read data from file
            cwc::glShader* pShader = nullptr;
            char vertexFilename[128], fragmentFilename[128], geometryFilename[128];
            sprintf(vertexFilename, "./Assets/%sVertexshader.txt", shaderName.data());
            sprintf(fragmentFilename, "./Assets/%sFragmentshader.txt", shaderName.data());
            sprintf(geometryFilename, "./Assets/%sGeometryshader.txt", shaderName.data());
            // there should be a specialized class for this whole thing
            cwc::glShaderManager shaderLoader;

            DWORD ret1 = GetFileAttributes((LPCWSTR)geometryFilename);
            int ret2 = GetLastError();
            bool isGeometryFileNotFound = (ret1 == INVALID_FILE_ATTRIBUTES) || (ret2 == ERROR_FILE_NOT_FOUND);

            // checks if the geometry shader actually is required
            if (isGeometryFileNotFound)
            {
                //File not found
                pShader = shaderLoader.loadfromFile(vertexFilename, fragmentFilename);
            }
            else
            {
                pShader = shaderLoader.loadfromFile(vertexFilename, geometryFilename, fragmentFilename);
            }


            if (pShader == nullptr)
            {
                printf("<!> Failed to parse shader files [%s]\n", shaderName.data());
                status = false;
            }

            // time measurement
            printf(" [*] loading shader [%s] %.2fms\n", shaderName.data(), (float)(clock() - start));

            return pShader;
        }
#pragma optimize( "", on )
	};

}
#endif // _OglHelper_H_