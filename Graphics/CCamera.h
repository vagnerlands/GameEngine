#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CommonTypes.h"
#include "IvVector3.h"

using namespace Types;

class CCamera 
{
public:
	// regular ctor - just initialize m_data
	CCamera();

	/*void prepareProjection2D();

	void prepareProjection3D();*/

	void SetLookAtMatrix();

	void RotateX(Float Angle);

	void RotateY(Float Angle);

	void RotateZ(Float Angle);

	void MoveForward(Float Distance);

	void MoveUpward(Float Distance);

	void MoveRight(Float Distance);
	
	void HoverForward(Float Distance);

	void HoverRight(Float Distance);

	//void SetCameraAttribute(CameraAttributeType attr, Float x, Float y, Float z);

	//void SetCameraAttribute(CameraAttributeType attr, IvVector3 xyz);

	// view/forward vector
	IvVector3 m_viewDir;
	// right vector
	IvVector3 m_rightVector;
	// upright vector
	IvVector3 m_upVector;
	// physical location of camera in carthesian plan
	IvVector3 m_position;
	// rotation angle accumulated on xyz
	IvVector3 m_rotation;


};


#endif //_CCAMERA_H_
