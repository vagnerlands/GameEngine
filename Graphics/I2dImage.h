#ifndef _I2DIMAGE_H_
#define _I2DIMAGE_H_

#include "CommonTypes.h"
#include "OGLTypes.h"

using namespace Types;
using namespace std;

struct SImageData
{
	Int32 m_Width;
	Int32 m_Height;
	Int32 m_BitCount;
	Int32 m_SizeInBytes;
	Byte* m_pContent;
};

class I2dImage
{
public:
	I2dImage();

	virtual bool ParseStream(Byte* pData, UInt32 length) = 0;

	virtual ~I2dImage() 
	{
		//empty implementation
	}

	virtual void SetCubeMap(bool isCubeMap)
	{
		m_isCubeMap = isCubeMap;
	}

	Int32 GetWidth() const;
	Int32 GetHeight() const;
	Int32 GetNumberOfBytes() const;
	Int32 GetSizeInBytes() const;
	Byte* GetPointerToData() const;

protected:
	Int32				m_Width;
	Int32				m_Height;
	Int32				m_NumberOfBytes;
	Int32				m_SizeInBytes;
	bool				m_isCubeMap;
	Byte*				m_pContent;
	
};



inline Int32 I2dImage::GetWidth() const
{
	return m_Width;
}

inline Int32 I2dImage::GetHeight() const
{
	return m_Height;
}

inline Int32 I2dImage::GetNumberOfBytes() const
{
	return m_NumberOfBytes;
}

inline Int32 I2dImage::GetSizeInBytes() const
{
	return m_SizeInBytes;
}

inline Byte* I2dImage::GetPointerToData() const
{
	return m_pContent;
}

#endif //_I2DIMAGE_H_