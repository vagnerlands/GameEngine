#include "CTextureOGL.h"
#include "I2dImage.h"
#include <GL/glew.h>

bool Graphics::CTextureOGL::BuildTexture(const std::shared_ptr<I2dImage>& pData)
{
	Int32 err = 0;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("Graphics::CTextureOGL::BuildTexture error cleaning... =%d\n", err);
	// allocate a texture name
	m_textureId = -1;
	// then, allocate an indexer for new texture
	glGenTextures(1, &m_textureId);
	// binds this texture as a 2D bitmap
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError BindTexture=%d\n", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	// always good to get the error in case it happens
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("glError Texture parameters=%d\n", err);
	}

    const UInt16 cNumOfChannels = (pData->GetNumberOfBytes() == 4) ? GL_RGBA : GL_RGB;

	// build our texture from the buffer data
    glTexImage2D(GL_TEXTURE_2D,
        0,
        cNumOfChannels,
        pData->GetWidth(),
        pData->GetHeight(),
        0,
        cNumOfChannels,
        GL_UNSIGNED_BYTE,
        pData->GetPointerToData());
	// checks for GL ERROR
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("Error creating texture - glTexImage2D=%d\n", err);
		glDeleteTextures(1, &m_textureId);
		m_status = false;
	}
	else
	{
        glGenerateMipmap(GL_TEXTURE_2D);
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
        }
	}

	return m_status;
}

bool Graphics::CTextureOGL::Bind()
{
	if (m_status)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		return true;
	}

	return false;
}

bool Graphics::CTextureOGL::BindTextureToSampler(UInt32 samplerId)
{
	if (m_status)
	{
		Int32 glerr = glGetError();

		glActiveTexture(GL_TEXTURE0 + samplerId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		return true;
	}

	return false;
}

Graphics::CTextureOGL::~CTextureOGL()
{
	if (m_status)
	{
		glDeleteTextures(1, &m_textureId);
	}

	m_status = false;
}
