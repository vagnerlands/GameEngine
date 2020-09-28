#include "DevILLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_DDS
#include "stb_image.h"

#include <iostream>

DevILLoader::DevILLoader()
{
}

bool DevILLoader::ParseStream(Byte * pData, UInt32 length)
{
	bool status = false;
	FILE* tFile;
	if (fopen_s(&tFile, "./rem.bmp", "wb+") != 0)
		return false;
	fwrite(pData, length, 1, tFile);
    fflush(tFile);
	fclose(tFile);

	Int32 width, height, nrComponents;
	m_pContent = (Byte*)stbi_load("./rem.bmp", &width, &height, &nrComponents, 0);
	if (m_pContent)
	{
		m_NumberOfBytes = nrComponents;
		// not including padding - but that's fine...
		m_SizeInBytes = nrComponents * width * height;
		// report the process succeeded
		status = true;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << std::endl;
		stbi_image_free(m_pContent);
		status = false;
	}


	return status;
}

DevILLoader::~DevILLoader()
{
	// make sure that the allocated data will be freed
	stbi_image_free(m_pContent);
}
