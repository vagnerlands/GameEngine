#include "CBmp.h"

CBmp::CBmp() 
{
}

bool CBmp::ParseStream(Byte* pData, UInt32 length)
{
	if (pData != NULL)
	{
		// retrieve header information
		// offset for start of bmp data
		Int32 dataPos = *(Int32*)&(pData[0x0A]);
		// image size in bytes
		m_SizeInBytes = *(Int32*)&(pData[0x22]);
		// width in pixels
		m_Width = *(Int32*)&(pData[0x12]);
		// height in pixels
		m_Height = *(Int32*)&(pData[0x16]);
		// supports only 24bits and 32bits
		m_NumberOfBytes = 3;
		// checks if this image is 32 bits
		if (m_SizeInBytes == (m_Width * m_Height * 4))
			m_NumberOfBytes = 4;
		const Int32 totalSizeInBytes = m_Width*m_Height*m_NumberOfBytes;
        // some image editors may not give the total size - so we have to calculate ourselves
        if (m_SizeInBytes == 0U)
        {
            m_SizeInBytes = totalSizeInBytes;
        }
		// data must be sent in BGRA order - must swap byte order of integer
		Byte* swapBuffer = new Byte[totalSizeInBytes];
		for (Int32 x = 0; x < totalSizeInBytes;)
		{
			if (m_NumberOfBytes == 4)
			{
				swapBuffer[x] = pData[x + dataPos + 2];
				swapBuffer[x + 1] = pData[x + dataPos + 1];
				swapBuffer[x + 2] = pData[x + dataPos + 0];
				swapBuffer[x + 3] = pData[x + dataPos + 3];
			}
			else if (m_NumberOfBytes == 3)
			{
				swapBuffer[x] = pData[x + dataPos + 2];
				swapBuffer[x + 1] = pData[x + dataPos + 1];
				swapBuffer[x + 2] = pData[x + dataPos];
			}
			x += m_NumberOfBytes;
		}

		m_pContent = swapBuffer;

		if (m_pContent == NULL)
		{
			delete[] swapBuffer;
			return false;
		}

		return true;
	}

	return false;
}

CBmp::~CBmp()
{
    if (m_pContent != nullptr)
    {
        delete[] m_pContent;
        m_pContent = 0;
    }
}
