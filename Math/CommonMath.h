//===============================================================================
//
//===============================================================================

#ifndef _CommonMath_h_
#define _CommonMath_h_

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "CommonTypes.h"
#include "IvMatrix44.h"
#include "IvVector3.h"
#include "IvMatrix33.h"

class CommonMath
{
public:
	static IvMatrix44 LookAt(const IvVector3& eye, const IvVector3& lookAt, const IvVector3& up)
	{
		// compute view vectors
		IvVector3 view = lookAt - eye;
		view.Normalize();

		IvVector3 right = view.Cross(up);
		right.Normalize();

		IvVector3 viewUp = right.Cross(view);
		viewUp.Normalize();

		// now set up matrices
		// world->view rotation
		IvMatrix33 rotate;
		//if (IvRenderer::mRenderer->GetAPI() == kOpenGL)
		{
			rotate.SetRows(right, viewUp, -view);
		}
		/*else
		{
			rotate.SetRows(right, viewUp, view);
		}*/

		// world->view translation
		IvVector3 xlate = -(rotate*eye);

		// build 4x4 matrix
        
		IvMatrix44 lookAtMat = IvMatrix44(rotate);
        lookAtMat(0, 3) = xlate.GetX();
        lookAtMat(1, 3) = xlate.GetY();
        lookAtMat(2, 3) = xlate.GetZ();

		return lookAtMat;
	}

};


#endif
