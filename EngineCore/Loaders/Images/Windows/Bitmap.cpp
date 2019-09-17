//-----------------------------------------------------------------------------
// Copyright (c) 2006-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// This header file contains the new OpenGL symbols and functions required to
// support multisample anti-aliasing (MSAA) and coverage sample anti-aliasing
// (CSAA) on the Windows platform.
//
// NVIDIA GeForce 8 series and higher GPUs support a new anti-aliasing method
// called coverage sample anti-aliasing (CSAA). CSAA is usually used with 
// framebuffer objects (FBOs) for offscreen rendering. However CSAA can also be
// used in an application window.
//
// The following extensions are used for MSAA:
//  GL_ARB_multisample
//  WGL_ARB_multisample
//  WGL_ARB_pixel_format
//
// The following NVIDIA extensions are used for CSAA:
//  GL_NV_multisample_coverage
//  WGL_NV_multisample_coverage
//
// Each function is loaded on first use. Ensure the extensions are supported
// via the OpenGL extension string and then start calling them.
//
// Three custom functions are provided to simplify (multisample and coverage
// sample) anti-aliasing setup under Windows:
//  ChooseBestAntiAliasingPixelFormat()
//  ChooseAntiAliasingPixelFormat()
//  GetAntiAliasingPixelFormatString()
//
// Use these functions in place of ChoosePixelFormat() to return a pixel format
// that supports MSAA.
//
// Example usage:
//  1. Get the highest quality MSAA pixel format.
//      int pf;
//
//      ChooseBestAntiAliasingPixelFormat(pf);
//
//      PIXELFORMATDESCRIPTOR pfd = { fill in as per normal };
//      HDC hDC = GetDC( your app's HWND );
//      HGLRC hRC;
//
//      SetPixelFormat(hDC, pf, &pfd);
//      hRC = wglCreateContext(hDC);
//      wglMakeCurrent(hDC, hRC);
//
//  2. Get a pixel format supporting 4x anti-aliasing.
//      int pf;
//
//      ChooseAntiAliasingPixelFormat(pf, 4);
//
//      PIXELFORMATDESCRIPTOR pfd = { fill in as per normal };
//      HDC hDC = GetDC( your app's HWND );
//      HGLRC hRC;
//
//      SetPixelFormat(hDC, pf, &pfd);
//      hRC = wglCreateContext(hDC);
//      wglMakeCurrent(hDC, hRC);
//
//-----------------------------------------------------------------------------
#include "Bitmap.h"

#pragma comment (lib, "olepro32.lib")   // for IPicture COM interface support
#pragma comment (lib, "Ole32.lib")

#include <windows.h>
#include <olectl.h.>    // for OleLoadPicture() and IPicture COM interface

namespace
{
#pragma pack(push, 1)

	// TGA file header structure. This *must* be byte aligned.
	struct TgaHeader
	{
		BYTE idLength;
		BYTE colormapType;
		BYTE imageType;
		WORD firstEntryIndex;
		WORD colormapLength;
		BYTE colormapEntrySize;
		WORD xOrigin;
		WORD yOrigin;
		WORD width;
		WORD height;
		BYTE pixelDepth;
		BYTE imageDescriptor;
	};

	// TGA file footer structure. This *must* be byte aligned.
	struct TgaFooter
	{
		LONG extensionAreaOffet;
		LONG developerDirectoryOffset;
		BYTE signature[18];
	};

#pragma pack(pop)
}

int m_logpixelsx = 0;
int m_logpixelsy = 0;
// matches constant in MFC CDC class
const int HIMETRIC_INCH = 2540; 

Bitmap::Bitmap()
{
}

bool Bitmap::ParseStream(Byte* pData, UInt32 length)
{

	HRESULT hr = 0;
	HANDLE hFile = 0;
	HGLOBAL hGlobal = 0;
	IStream *pIStream = 0;
	IPicture *pIPicture = 0;
	BYTE *pBuffer = 0;
	DWORD dwFileSize = length;
	DWORD dwBytesRead = 0;
	LONG lWidth = 0;
	LONG lHeight = 0;

	if (!m_logpixelsx && !m_logpixelsy)
	{
		HDC hScreenDC = CreateCompatibleDC(GetDC(0));

		if (!hScreenDC)
			return false;

		m_logpixelsx = GetDeviceCaps(hScreenDC, LOGPIXELSX);
		m_logpixelsy = GetDeviceCaps(hScreenDC, LOGPIXELSY);
		DeleteDC(hScreenDC);
	}

	FILE* tFile;
	if (fopen_s(&tFile, "./rem.bin", "w+") != 0)
		return false;
	fwrite(pData, length, 1, tFile);
	fclose(tFile);

	hFile = CreateFile(L"./rem.bin", FILE_READ_DATA, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	if (!(dwFileSize = GetFileSize(hFile, 0)))
	{
		CloseHandle(hFile);
		return false;
	}

	if (!(hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwFileSize)))
	{
		CloseHandle(hFile);
		return false;
	}

	if (!(pBuffer = reinterpret_cast<BYTE*>(GlobalLock(hGlobal))))
	{
		GlobalFree(hGlobal);
		CloseHandle(hFile);
		return false;
	}

	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, 0))
	{
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		CloseHandle(hFile);
		return false;
	}

	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	if (FAILED(CreateStreamOnHGlobal(hGlobal, FALSE, &pIStream)))
	{
		GlobalFree(hGlobal);
		return false;
	}

	if (FAILED(OleLoadPicture(pIStream, 0, FALSE, IID_IPicture,
		reinterpret_cast<LPVOID*>(&pIPicture))))
	{
		pIStream->Release();
		GlobalFree(hGlobal);
		return false;
	}

	pIStream->Release();
	GlobalFree(hGlobal);

	pIPicture->get_Width(&lWidth);
	pIPicture->get_Height(&lHeight);

	UInt32 width = MulDiv(lWidth, m_logpixelsx, HIMETRIC_INCH);
	UInt32 height = MulDiv(lHeight, m_logpixelsy, HIMETRIC_INCH);

	if (!create(width, height))
	{
		pIPicture->Release();
		return false;
	}

	//selectObject();
	//hr = pIPicture->Render(dc, 0, 0, m_Width, m_Height, 0, lHeight, lWidth, -lHeight, 0);
	//deselectObject();

	pIPicture->Release();
	return (SUCCEEDED(hr)) ? true : false;
}

bool Bitmap::create(UInt32 width, UInt32 height)
{
	destroy();

	m_Width = width;
	m_Height = height;

	pitch = ((m_Width * 32 + 31) & ~31) >> 3;
	dc = CreateCompatibleDC(0);

	if (!dc)
		return false;

	memset(&info, 0, sizeof(info));

	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = m_Width;
	info.bmiHeader.biHeight = -m_Height;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biPlanes = 1;

	hBitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,
		reinterpret_cast<void**>(&m_pContent), 0, 0);

	if (!hBitmap)
	{
		destroy();
		return false;
	}

	GdiFlush();
	return true;
}

Bitmap::~Bitmap()
{
	destroy();
	/*if (m_pContent != nullptr)
	{
		delete[] m_pContent;
		m_pContent = 0;
	}*/
}
