#include "CCamera.h"
#include <iostream>
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

CCamera::CCamera() :
	m_position(0.0f, 0.0f, 0.0f),
	m_rightVector(1.0f, 0.0f, 0.0f),
	m_upVector(0.0f, 1.0f, 0.0f),
	m_viewDir(0.0f, 0.0f, 1.0f),
	m_rotation(0.0f, 0.0f, 0.0f)	
{
	m_camRotation.Identity();
}
/*
void CCamera::prepareProjection3D()
{
	// prepares PERSPECTIVE PROJECTION
	glMatrixMode(GL_PROJECTION);
	//Reset the drawing perspective
	glLoadIdentity(); 
	static GLfloat frustumParams[] = { -1.f, 1.f, -1.f, 1.f, 5.f, 10000.f };
	// prepares perspective projection
	glFrustum(frustumParams[0],
		frustumParams[1],
		frustumParams[2],
		frustumParams[3],
		frustumParams[4],
		frustumParams[5]);
}

void CCamera::prepareProjection2D()
{
	glMatrixMode(GL_MODELVIEW);
	// prepares ORTHO PROJECTION
	glMatrixMode(GL_PROJECTION);
	//Reset the drawing perspective
	glLoadIdentity(); 
	// prepares orthogonal projection
	glOrtho(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), -600.0, 600.0);
}*/

void CCamera::SetLookAtMatrix()
{
	//IvVector3 viewPoint = m_position + m_viewDir;

	//gluLookAt(m_position.x, m_position.y, m_position.z, viewPoint.x, viewPoint.y, viewPoint.z, m_upVector.x, m_upVector.y, m_upVector.z);
}

void CCamera::RotateX(Float Angle)
{
	Angle *= 0.1;
	m_rotation.SetX(m_rotation.GetX() + Angle);

	//printf("[ang=%f totalangX = %f] - ", Angle, m_rotation.GetX());

	//Rotate viewdir around the right vector:
	m_viewDir = (m_viewDir * cos(Angle*PIdiv180) + m_upVector * sin(Angle*PIdiv180));
	m_viewDir.Normalize();

	//now compute the new UpVector (by cross product)
	m_upVector = m_viewDir.Cross(m_rightVector) * -1;
	// try to keep the head up
	m_upVector.SetY(1.0f);

	// new approach
	IvMatrix33 rotate;
	rotate.RotationX(Angle);
	m_camRotation = m_camRotation * rotate;
	//m_camRotation[1] = 0.F;
	//m_camRotation[4] = 1.F;
	//m_camRotation[7] = 0.F;
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
	m_rotation.SetY(m_rotation.GetY() + Angle);
	//printf("[ang=%f totalangY = %f]\n\n", Angle, m_rotation.GetY());

	//Rotate viewdir around the up vector:
	m_viewDir = (m_viewDir * cos(Angle * PIdiv180) - m_rightVector * sin(Angle * PIdiv180));
	m_viewDir.Normalize();

	//now compute the new RightVector (by cross product)
	m_rightVector = m_viewDir.Cross(m_upVector);

	// new approach
	IvMatrix33 rotate;
	rotate.RotationY(Angle);
	m_camRotation = m_camRotation * rotate;
	//m_camRotation[1] = 0.F;
	//m_camRotation[4] = 1.F;
	//m_camRotation[7] = 0.F;
	std::cout << "   Y " << Angle << std::endl;
}

void CCamera::RotateZ(Float Angle)
{
	Angle *= 0.1;
	m_rotation.SetZ(m_rotation.GetZ() + Angle);

	//Rotate viewdir around the right vector:
	m_rightVector = (m_rightVector * cos(Angle * PIdiv180) + m_upVector * sin(Angle * PIdiv180));
	m_rightVector.Normalize();

	//now compute the new UpVector (by cross product)
	m_upVector = m_viewDir.Cross(m_rightVector) * -1;


	// new approach
	IvMatrix33 rotate;
	rotate.RotationZ(Angle);
	m_camRotation = m_camRotation * rotate;
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
