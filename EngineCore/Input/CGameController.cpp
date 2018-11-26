#include "CGameController.h"
#include <math.h>

CGameController::CGameController(Int32 centerOfScreen_X, Int32 centerOfScreen_Y) :
	mCenterOfScreen_X(centerOfScreen_X/2),
	mCenterOfScreen_Y(centerOfScreen_Y/2)
{
	for (Int32 i = 0; i < 256; i++)
	{
		m_bKey[i] = false;
	}
}

CGameController::~CGameController()
{
	// empty implementation
}

void 
CGameController::OnUpdate()
{

}

bool 
CGameController::VOnMouseMove(const IvPoint &mousePos)
{
	if (m_lastPos != mousePos)
	{
		// radians factor for convertion
		static const Float s_RadiansFactor = 180.0f/ 3.14159;

		// mouse coordinate difference between one cycle and another
		float deltaX = 0;
		float deltaY = 0;

		deltaX = m_lastPos.x - mousePos.x;
		deltaY = m_lastPos.y - mousePos.y;

		float angle = (int)((atan2(deltaY, deltaX) * s_RadiansFactor) + 90.F) % 360;
		// intensity of the movement (how many pixels the cursor has moved from one cycle to another)
		m_movementIntensityX = deltaX;
		m_movementIntensityY = deltaY;

		//if (m_movementIntensityX > 0) m_movementIntensityX = -m_movementIntensityX;
		//if (m_movementIntensityY < 0) m_movementIntensityY = -m_movementIntensityY;

		// angle of the cursor movement
		m_movementAngle = angle;
		// distance from screen center
		m_distanceFromCenter = sqrt(pow(mCenterOfScreen_X - mousePos.x, 2.0) + pow(mCenterOfScreen_Y - mousePos.y, 2.0));
		// angle from center of the screen
		m_angleFromCenter = (Int32)((atan2(mousePos.y - mCenterOfScreen_Y, mousePos.x - mCenterOfScreen_X) * s_RadiansFactor) + 450.0F) % 360;

		printf("angle=%d, distCenter = %d, angleCenter = %d\n",
			m_movementAngle,
			m_distanceFromCenter,
			m_angleFromCenter);

		m_lastPos = mousePos;
	}

	return true;
}
