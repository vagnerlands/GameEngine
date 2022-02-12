//===============================================================================
// @ InvalidTexture.h
// Holds a BMP file content (binary) with a default texture (like chess board)
//===============================================================================

#ifndef _InvalidTexture_H_
#define _InvalidTexture_H_

#include "CommonTypes.h"

using namespace Types;

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{
	struct InvalidTexture
	{
		static UByte Value[49206];
	};
}

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------


#endif // _InvalidTexture_H_