#include "CTga.h"

CTga::CTga()
{
}

bool CTga::ParseStream(Byte* pData, UInt32 length)
{
	if (pData != NULL)
	{
		STgaFormat* dataStream = reinterpret_cast<STgaFormat*>(pData);
		// 3 is RGB, 4 is RGBA
		m_NumberOfBytes = dataStream->bitCount / 8;
		m_Width = dataStream->imageWidth;
		m_Height = dataStream->imageHeight;
		if ((m_Width < 0) || (m_Height < 0) || ((m_NumberOfBytes != 3) && (m_NumberOfBytes != 4)))
		{
			printf("Tga file is invalid\n");
			return false;
		}
		// constant - offset start of the image data (pixels info)
		const Int32 imageContentOffset = sizeof(STgaFormat);
		// constant - image size in bytes
		const Int32 imageSize = m_Width * m_Height * m_NumberOfBytes;
		// pixels
		m_pContent = new Byte[imageSize];
		// is this image not compressed?
		if (dataStream->imageTypeCode == TGA_RGB || dataStream->imageTypeCode == TGA_GRAYSCALE)
		{
			memcpy(m_pContent, pData + dataStream->idLength, imageSize);
		}
		else
		{
			// this is an RLE compressed image
			UByte id;
			UByte length;
			SRgba color = { 0, 0, 0, 0 };
			UInt32 i = 0;
			UInt32 streamOffset = dataStream->idLength;

			while (i < imageSize)
			{
				id = pData[streamOffset++];
				// see if this is run length data
				if (id >= 128)// & 0x80)
				{
					// find the run length
					length = (UByte)(id - 127);

					// next 3 (or 4) bytes are the repeated values
					color.b = (UByte)pData[streamOffset++];
					color.g = (UByte)pData[streamOffset++];
					color.r = (UByte)pData[streamOffset++];

					if (m_NumberOfBytes == 4)
						color.a = (UByte)pData[streamOffset++];

					// save everything in this run
					while (length > 0)
					{
						m_pContent[i++] = color.b;
						m_pContent[i++] = color.g;
						m_pContent[i++] = color.r;

						if (m_NumberOfBytes == 4)
							m_pContent[i++] = color.a;

						--length;
					}
				}
				else
				{
					// the number of non RLE pixels
					length = (UByte)(id + 1);

					while (length > 0)
					{
						color.b = (UByte)pData[streamOffset++];
						color.g = (UByte)pData[streamOffset++];
						color.r = (UByte)pData[streamOffset++];

						if (m_NumberOfBytes == 4)
							color.a = (UByte)pData[streamOffset++];

						m_pContent[i++] = color.b;
						m_pContent[i++] = color.g;
						m_pContent[i++] = color.r;

						if (m_NumberOfBytes == 4)
							m_pContent[i++] = color.a;

						--length;
					}
				}
			}
		}

		for (Int32 imageIdx = 0; imageIdx < imageSize; imageIdx += m_NumberOfBytes)
		{
			Byte colorSwap = pData[imageContentOffset + imageIdx];
			pData[imageContentOffset + imageIdx] = pData[imageContentOffset + imageIdx + 2];
			pData[imageContentOffset + imageIdx + 2] = colorSwap;
		}

		//memcpy(m_pContent, dataStream.imageData, m_Width * m_Height * m_NumberOfBytes);
		//m_pContent = reinterpret_cast<Byte*>(&dataStream.imageData);

		//m_pContent = swapBuffer;

		if (m_pContent == NULL)
		{
			//delete[] swapBuffer;
			return false;
		}

		return true;
	}

	return false;
}

CTga::~CTga()
{
    if (m_pContent != nullptr)
    {
        delete[] m_pContent;
        m_pContent = 0;
    }
}
