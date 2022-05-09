#include "CCamera.h"
#include <iostream>

#include "glm\glm.hpp"

#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

CCamera::CCamera() :
	m_position(0.0f, 0.0f, 0.0f),
	m_rightVector(1.0f, 0.0f, 0.0f),
	m_upVector(0.0f, 1.0f, 0.0f),
	m_viewDir(0.0f, 0.0f, -1.0f),
	m_rotation(0.0f, 0.0f, 0.0f)
{
	m_camRotation.SetColumns(m_rightVector, m_upVector, m_viewDir);
}

void CCamera::SetLookAtMatrix()
{
    cout << "CCamera::SetLookAtMatrix() is not implemented" << endl;
}

void CCamera::RotateX(Float Angle)
{
	Angle *= 0.1f;

	// rotate the camera based on given angle (use quaternion)
	IvMatrix33 rotate;
	rotate.RotationX(Angle);
	m_camRotation = m_camRotation * rotate;

	// keep camera "head-up", without tilting it 
	removeCameraRoll();
}

void CCamera::RotateY(Float Angle)
{
	Angle *= 0.1f;

    // rotate the camera based on given angle (use quaternion)
	IvMatrix33 rotate;
	rotate.RotationY(Angle);
	m_camRotation = m_camRotation * rotate;
    // keep camera "head-up", without tilting it 
	removeCameraRoll();
}

void CCamera::RotateZ(Float Angle)
{
	Angle *= 0.1f;

    // rotate the camera based on given angle (use quaternion)
	IvMatrix33 rotate;
	rotate.RotationZ(Angle);
	m_camRotation = m_camRotation * rotate;
    // keep camera "head-up", without tilting it 
	removeCameraRoll();
}

void CCamera::MoveForward(Float Distance)
{
	IvVector3 viewDir = m_camRotation.GetColumn(2);
	//m_position = m_position + (m_viewDir * (-Distance));
	m_position = m_position + (viewDir * (-Distance));
    //cout << " Camera x: " << m_position.GetX() << ", y: " << m_position.GetY() << ", z: " << m_position.GetZ() << endl;
}

void CCamera::MoveUpward(Float Distance)
{
	IvVector3 upVector = m_camRotation.GetColumn(1);
	m_position = m_position + (upVector * Distance);
    //cout << " Camera x: " << m_position.GetX() << ", y: " << m_position.GetY() << ", z: " << m_position.GetZ() << endl;
}

void CCamera::MoveRight(Float Distance)
{
	IvVector3 rightVector = m_camRotation.GetColumn(0);
	m_position = m_position + (rightVector* Distance);
    cout << " Camera x: " << m_position.GetX() << ", y: " << m_position.GetY() << ", z: " << m_position.GetZ() << endl;
}

void CCamera::HoverForward(Float Distance)
{
	IvVector3 direction = m_position + (m_viewDir * Distance);
	m_position = IvVector3(direction.GetX(), m_position.GetY(), direction.GetZ());
}

void CCamera::HoverRight(Float Distance)
{
	IvVector3 direction = m_position + (m_rightVector * Distance);
	m_position = IvVector3(direction.GetX(), m_position.GetY(), direction.GetZ());
}

void CCamera::SetCameraType(ECameraType camera_type)
{
    m_type = camera_type;

}

void CCamera::removeCameraRoll()
{
	if ((m_type == ECameraType::Camera_Human)
		|| (m_type == ECameraType::Camera_Spectator))
	{
		// temporary vectors 
		IvVector3 right, up, forward;
		// retrieve the 3 vectors that compose the "View Matrix"
		m_camRotation.GetColumns(right, up, forward);
		// ignore the Y orientation of the right vector
		right.SetY(0);
		// calculates the new Up vector, cross product will return a perpendicular vector to forward X right (with head-up)
		up = forward.Cross(right) * -1.f;
		// normalize this given vector to avoid anormalities
		up.Normalize();
		// now that we have the actual up vector perpendicular to forward X "pseudo"-right, we calculate
		// the correct right vector
		right = forward.Cross(up);
		// update the camera rotation with the new vectors
		m_camRotation.SetColumns(right, up, forward);
	}
}