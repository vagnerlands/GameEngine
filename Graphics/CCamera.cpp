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
	//IvVector3 viewPoint = m_position + m_viewDir;

	//gluLookAt(m_position.x, m_position.y, m_position.z, viewPoint.x, viewPoint.y, viewPoint.z, m_upVector.x, m_upVector.y, m_upVector.z);
}

void CCamera::RotateX(Float Angle)
{
	Angle *= 0.1;

	//m_rotation.SetX(m_rotation.GetX() + Angle);

	////printf("[ang=%f totalangX = %f] - ", Angle, m_rotation.GetX());

	////Rotate viewdir around the right vector:
	//m_viewDir = (m_viewDir * cos(Angle*PIdiv180) + m_upVector * sin(Angle*PIdiv180));
	//m_viewDir.Normalize();

	////now compute the new UpVector (by cross product)
	////m_upVector    = m_viewDir.Cross(m_rightVector) * -1;
	//m_rightVector = m_viewDir.Cross(m_upVector) * -1;
	//m_rightVector.Normalize();

	// new approach
	IvMatrix33 rotate;
	rotate.RotationX(Angle);
	m_camRotation = m_camRotation * rotate;

	// ignore the Roll of the camera
	removeCameraRoll();

	for (int i = 0; i < 9; ++i)
		if (i%3 == 0)
			std::cout << std::endl << "  "  << m_camRotation[i] ;
		else
			std::cout << "  " << m_camRotation[i];
	std::cout << std::endl;
}

void CCamera::RotateY(Float Angle)
{
	Angle *= 0.1;
	//m_rotation.SetY(m_rotation.GetY() + Angle);
	////printf("[ang=%f totalangY = %f]\n\n", Angle, m_rotation.GetY());

	////Rotate viewdir around the up vector:
	//m_viewDir = (m_viewDir * cos(Angle * PIdiv180) - m_rightVector * sin(Angle * PIdiv180));
	//m_viewDir.Normalize();

	////now compute the new RightVector (by cross product)
	////m_rightVector = m_viewDir.Cross(m_upVector) * -1;
	//m_rightVector = m_viewDir.Cross(m_upVector) * -1;
	//m_rightVector.Normalize();
	////m_upVector    = m_viewDir.Cross(m_rightVector) * -1;

	// new approach
	IvMatrix33 rotate;
	rotate.RotationY(Angle);
	m_camRotation = m_camRotation * rotate;
	// ignore the Roll of the camera
	removeCameraRoll();

	std::cout << "   Y " << Angle << std::endl;
}

void CCamera::RotateZ(Float Angle)
{
	Angle *= 0.1;
	//m_rotation.SetZ(m_rotation.GetZ() + Angle);

	////Rotate viewdir around the right vector:
	//m_rightVector = (m_rightVector * cos(Angle * PIdiv180) + m_upVector * sin(Angle * PIdiv180));
	//m_rightVector.Normalize();

	////now compute the new UpVector (by cross product)
	//m_upVector = m_viewDir.Cross(m_rightVector) * -1;


	// new approach
	IvMatrix33 rotate;
	rotate.RotationZ(Angle);
	m_camRotation = m_camRotation * rotate;

	// ignore the Roll of the camera
	removeCameraRoll();

	std::cout << "    Z" << std::endl;
}

void CCamera::MoveForward(Float Distance)
{
	IvVector3 viewDir = m_camRotation.GetColumn(2);
	//m_position = m_position + (m_viewDir * (-Distance));
	m_position = m_position + (viewDir * (-Distance));
}

void CCamera::MoveUpward(Float Distance)
{
	IvVector3 upVector = m_camRotation.GetColumn(1);
	//m_position = m_position + (m_upVector * Distance);
	m_position = m_position + (upVector * Distance);
}

void CCamera::MoveRight(Float Distance)
{
	IvVector3 rightVector = m_camRotation.GetColumn(0);
	//m_position = m_position + (m_rightVector * Distance);
	m_position = m_position + (rightVector* Distance);
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

void CCamera::removeCameraRoll()
{
	if ((m_type == Camera_Human) 
		|| (m_type == Camera_Spectator))
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

/*void CCamera::SetCameraAttribute(CameraAttributeType attr, TFloat x, TFloat y, TFloat z)
{
	switch (attr)
	{
	case CameraAttributeType::CameraAttribute_Forward:
		m_viewDir.x = x;
		m_viewDir.y = y;
		m_viewDir.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Up:
		m_upVector.x = x;
		m_upVector.y = y;
		m_upVector.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Right:
		m_rightVector.x = x;
		m_rightVector.y = y;
		m_rightVector.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Position:
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		break;
	}
}

void CCamera::SetCameraAttribute(CameraAttributeType attr, glm::vec3 xyz)
{
	switch (attr)
	{
	case CameraAttributeType::CameraAttribute_Forward:
		m_viewDir = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Up:
		m_upVector = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Right:
		m_rightVector = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Position:
		m_position = xyz;
		break;
	}
}*/
