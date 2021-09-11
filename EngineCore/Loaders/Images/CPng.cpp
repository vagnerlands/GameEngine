#include "CPng.h"

CPng::CPng() : m_pImageDataBuffer(nullptr)
{
}

bool CPng::ParseStream(Byte* pData, UInt32 length)
{
    bool retval = false;
	if (pData != NULL)
	{
        // png image struct
        png_image image;        
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        if (png_image_begin_read_from_memory(&image, reinterpret_cast<void*>(pData), static_cast<UInt32>(length)) != 0)
        {
            image.format = PNG_FORMAT_RGBA;
            png_bytep buffer; // unpacked buffer
            m_SizeInBytes = PNG_IMAGE_SIZE(image);
            m_pContent = new Byte[m_SizeInBytes];
            buffer = (png_bytep)malloc(m_SizeInBytes);
            m_Width = image.width;
            m_Height = image.height;
            m_NumberOfBytes = 4;            
            if (png_image_finish_read(&image, 0, buffer, 0, 0) != 0) // copies the unpacked png bitmap to the buffer
            {
                m_pImageDataBuffer = (Byte*)buffer; // assigns reference to broader variable
                copyPngDataToOutputBuffer(image.width, image.height, m_SizeInBytes, (Byte*)m_pContent); // inverts the Y axis of the png
                retval = true;
            }
            free(buffer); // release buffer in any case
        }
	}

	return retval;
}

CPng::~CPng()
{
    if (m_pContent != nullptr)
    {
        delete[] m_pContent;
        m_pContent = 0;
    }
}

//! \internal Constructor
void
CPng::copyPngDataToOutputBuffer(UInt32 imageWidth, UInt32 imageHeight, UInt32 pixelDataSize, Byte* pPixelData) 
{
    // Png red index
    static const UInt32 PNG_RED_INDEX = 0U;
    // Png green index
    static const UInt32 PNG_GREEN_INDEX = 1U;
    // Png blue index
    static const UInt32 PNG_BLUE_INDEX = 2U;
    // Png alpha index
    static const UInt32 PNG_ALPHA_INDEX = 3U;

    static const UInt32 Uint32SizeInBytes = sizeof(UInt32);

    // Initialize the output buffer to 0. This will set any extra padding at the end of rows to 0
    memset(reinterpret_cast<void*>(pPixelData), 0, pixelDataSize); // parasoft-suppress  JSF-182 "On this case impossible to avoid pointer casting."

    // Single pixel data to copy
    Byte pixel[Uint32SizeInBytes];
    // Image row size in bytes
    UInt32 imageRowInBytes = imageWidth * static_cast<UInt32>(Uint32SizeInBytes);
    // Total bytes written to output buffer
    UInt32 bytesWritten = 0;
    // current pixel data location
    Byte* pPixelDataLocation = nullptr;
    // row start location in output buffer
    Byte* pRowStartLocation = nullptr;
    // current row byte offset in input buffer
    UInt32 rowOffsetInBytes = 0U;
    // current column byte offset in input buffer
    UInt32 colOffsetInBytes = 0U;

    // Loop through the image rows
    for (UInt32 row = 0; row < imageHeight; ++row)
    {
        pRowStartLocation = pPixelData + row * imageRowInBytes; // parasoft-suppress  JSF-215 "On this case impossible to avoid pointer arithmetic to pointer."
        rowOffsetInBytes = (imageHeight - row - 1U) * imageRowInBytes;
        // Loop through the image columns
        for (UInt32 column = 0; column < imageWidth; ++column)
        {
            colOffsetInBytes = column * static_cast<UInt32>(Uint32SizeInBytes);

            pixel[0] = m_pImageDataBuffer[rowOffsetInBytes + colOffsetInBytes + PNG_RED_INDEX];
            pixel[1] = m_pImageDataBuffer[rowOffsetInBytes + colOffsetInBytes + PNG_GREEN_INDEX];
            pixel[2] = m_pImageDataBuffer[rowOffsetInBytes + colOffsetInBytes + PNG_BLUE_INDEX];
            pixel[3] = m_pImageDataBuffer[rowOffsetInBytes + colOffsetInBytes + PNG_ALPHA_INDEX];

            // Copy pixel to output buffer
            if (pixelDataSize - bytesWritten >= static_cast<UInt32>(Uint32SizeInBytes))
            {
                pPixelDataLocation = pRowStartLocation + colOffsetInBytes; // parasoft-suppress  JSF-215 "On this case impossible to avoid pointer arithmetic to pointer."
                memcpy(reinterpret_cast<void*>(pPixelDataLocation), pixel, static_cast<UInt32>(Uint32SizeInBytes)); // parasoft-suppress  JSF-182 "On this case impossible to avoid pointer casting."
                bytesWritten += static_cast<UInt32>(Uint32SizeInBytes);
            }
        }
    }
}
