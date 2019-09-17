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

#ifndef _Bitmap_H_
#define _Bitmap_H_

#include "I2dImage.h"

using namespace Types;
using namespace std;


class Bitmap : public I2dImage
{
public:
	Bitmap();

	virtual bool ParseStream(Byte* pData, UInt32 length) override;

	virtual ~Bitmap();

private:
	HDC dc;
	HGDIOBJ m_hPrevObj;
	HBITMAP hBitmap;
	Int32 pitch;
	BITMAPINFO info;

	bool create(UInt32 width, UInt32 height);

	void selectObject()
	{
		if (dc)
			m_hPrevObj = SelectObject(dc, hBitmap);
	}

	void deselectObject()
	{
		if (dc && m_hPrevObj)
		{
			SelectObject(dc, m_hPrevObj);
			m_hPrevObj = 0;
		}
	}

	void destroy()
	{
		deselectObject();

		if (hBitmap)
		{
			DeleteObject(hBitmap);
			hBitmap = 0;
		}

		if (dc)
		{
			DeleteDC(dc);
			dc = 0;
		}

		m_Width = m_Height = 0;
		m_hPrevObj = 0;
		m_pContent = 0;
	}

	
};

#endif //_Bitmap_H_