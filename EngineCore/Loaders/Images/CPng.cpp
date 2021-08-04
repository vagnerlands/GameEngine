#include "CPng.h"

CPng::CPng()
{
}

bool CPng::ParseStream(Byte* pData, UInt32 length)
{
	if (pData != NULL)
	{
		enum { kPngSignatureLength = 8 };
		Byte pngSignature[kPngSignatureLength];
		memcpy((void*)pngSignature, (void const*)pData, kPngSignatureLength);
		if (!png_check_sig((png_const_bytep)pngSignature, kPngSignatureLength))
			return false;

		// get PNG file info struct (memory is allocated by libpng)
		png_structp png_ptr = NULL;
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (png_ptr == NULL)
			return false;

		// get PNG image data info struct (memory is allocated by libpng)
		png_infop info_ptr = NULL;
		info_ptr = png_create_info_struct(png_ptr);

		if (info_ptr == NULL)
		{
			// libpng must free file info struct memory before we bail
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return false;
		}

		png_set_read_fn(png_ptr, pData + 8, CPng::ReadDataFromInputStream);

		// let the libPng knows that it should start parsing from the first byte of the stream
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 width = 0;
		png_uint_32 height = 0;
		int bitDepth = 0;
		int colorType = -1;
		png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
			&width,
			&height,
			&bitDepth,
			&colorType,
			NULL, NULL, NULL);

		if (retval != 1)
			return false;	// add error handling and cleanup

		png_uint_32 channels = png_get_channels(png_ptr, info_ptr);

		switch (colorType) 
		{
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png_ptr);
			//Don't forget to update the channel info (thanks Tom!)
			//It's used later to know how big a buffer we need for the image
			channels = 3;
			break;
		case PNG_COLOR_TYPE_GRAY:
			if (bitDepth < 8)
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			//And the bitdepth info
			bitDepth = 8;
			break;
		}

		/*if the image has a transperancy set.. convert it to a full Alpha channel..*/
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha(png_ptr);
			channels += 1;
		}

		//We don't support 16 bit precision.. so if the image Has 16 bits per channel
		//precision... round it down to 8.
		if (bitDepth == 16)
			png_set_strip_16(png_ptr);

		//As Nicholas suggested, we should let png update the information structs with the transformations we requested:
		png_read_update_info(png_ptr, info_ptr);

		// [ Reading Data]
		//Here's one of the pointers we've defined in the error handler section:
		//Array of row pointers. One for every row.
		png_bytep* rowPtrs = new png_bytep[height];

		//Alocate a buffer with enough space.
		//(Don't use the stack, these blocks get big easilly)
		//This pointer was also defined in the error handling section, so we can clean it up on error.
		m_NumberOfBytes = (bitDepth * channels) / 8;
		m_SizeInBytes = width * height * m_NumberOfBytes;
		m_pContent = new Byte[m_SizeInBytes];
		m_Width = width;
		m_Height = height;		
		//This is the length in bytes, of one row.
		const unsigned int stride = width * bitDepth * channels / 8;

		//A little for-loop here to set all the row pointers to the starting
		//Adresses for every row in the buffer

		for (size_t i = 0; i < height; i++) {
			//Set the pointer to the data pointer + i times the row stride.
			//Notice that the row order is reversed with q.
			//This is how at least OpenGL expects it,
			//and how many other image loaders present the data.
			png_uint_32 q = (height - i - 1) * stride;
			rowPtrs[i] = (png_bytep)m_pContent + q;
		}

		//And here it is! The actuall reading of the image!
		//Read the imagedata and write it to the adresses pointed to
		//by rowptrs (in other words: our image databuffer)
		png_read_image(png_ptr, rowPtrs);
		// [ End of Reading Data]
		
		// release data
		delete[] rowPtrs;
		// release all junk from png
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		
		return true;
	}

	return false;
}

CPng::~CPng()
{
    if (m_pContent != nullptr)
    {
        delete[] m_pContent;
        m_pContent = 0;
    }
}

// method to treat the readed information from PNG
void CPng::ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
	static Int32 cummulativeOffset = 0;
	png_voidp io_ptr = png_get_io_ptr(png_ptr);

	if (io_ptr == NULL)
		return;   // add custom error handling here

	// using pulsar::InputStream
	// -> replace with your own data source interface
	Byte* inputStream = reinterpret_cast<Byte*>(io_ptr);

	memcpy((void*)outBytes, inputStream + cummulativeOffset, byteCountToRead);

	cummulativeOffset += byteCountToRead;

	/*const size_t bytesRead = inputStream.Read(
		(byte*)outBytes,
		(size_t)byteCountToRead);

	if ((png_size_t)bytesRead != byteCount)
		return;   // add custom error handling here*/
}
