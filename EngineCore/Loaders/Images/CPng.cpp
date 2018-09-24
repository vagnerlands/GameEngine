#include "CPng.h"

CPng::CPng()
{
}

bool CPng::ParseStream(Byte* pData)
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

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		
		return true;
	}

	return false;
}

CPng::~CPng()
{
	if (m_pContent != NULL)
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
