#include "CCubeTextureOGL.h"
#include "I2dImage.h"
#include <GL/glew.h>

bool Graphics::CCubeTextureOGL::BuildVectorTexture(const I2dImage** pData)
{
	// allocate a texture name
	m_textureId = -1;
	// then, allocate an indexer for new texture
	glGenTextures(1, &m_textureId);
	// binds this texture as a 2D bitmap
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
	Int32 err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError BindTexture=%d\n", err);
	}

	// the 6 faces of the cube
	for (GLuint i = 0; i < 6; i++)
	{
		if (pData[i]->GetNumberOfBytes() == 4)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				pData[i]->GetWidth(),
				pData[i]->GetHeight(),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				pData[i]->GetPointerToData());
		}
		else
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				pData[i]->GetWidth(),
				pData[i]->GetHeight(),
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pData[i]->GetPointerToData());
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	// always good to get the error in case it happens	
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("Error creating texture - glTexImage2D=%d\n", err);
		glDeleteTextures(1, &m_textureId);
		m_status = false;
	}
	else
	{
		m_status = true;
        /*glGenerateMipmap(GL_TEXTURE_2D);
        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("Error creating mipmap - glGenerateMipMap=%d\n", err);
            glDeleteTextures(1, &m_textureId);
            m_status = false;
        }
        else
        {
		    m_status = true;
        }*/
	}

	return m_status;
}

bool Graphics::CCubeTextureOGL::Bind()
{
	if (m_status)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_CLAMP_TO_EDGE);
		return true;
	}

	return false;
}

bool Graphics::CCubeTextureOGL::BindTextureToSampler(UInt32 samplerId)
{
	if (m_status)
	{
		Int32 glerr = glGetError();

		glActiveTexture(GL_TEXTURE0 + samplerId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_CLAMP_TO_EDGE);
		return true;
	}

	return false;
}

Graphics::CCubeTextureOGL::~CCubeTextureOGL()
{
	if (m_status)
	{
		glDeleteTextures(1, &m_textureId);
	}

	m_status = false;
}
