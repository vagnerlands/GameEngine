//===============================================================================
// @ CClockOGL.cpp
// 
// Concrete implementation of IClock for OpenGL 
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "CClockOGL.h"
#include "GL/glut.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

UtilitiesCore::CClockOGL::CClockOGL()
{

}

UtilitiesCore::CClockOGL::~CClockOGL()
{
	// do nothing here
}

void UtilitiesCore::CClockOGL::StartFrame()
{
	// last measured time [debug] - local static variable
	static Float lastTime = getTimeNow();
	// number of frames [debug] - local static variable
	static Int32 nbFrames = 0;

	// find the current time according to CPU
	Float now = getTimeNow();
	// increase number of frames
	nbFrames++;
	// debug
	if (now - lastTime >= 1.0) 
	{ 
		//printf("%f frames per second\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
	// current time is the delta between last frame measurement and the current measurement
	mElapsedTime = now - mStartFrameTime;
	// current time is now (for temporary calculations)
	mCurrentTime = now;
	// start frame time is now
	mStartFrameTime = now;
}

inline Int32 UtilitiesCore::CClockOGL::FramesPerSecond() const
{
    return mFps.GetFramesPerSecond();
}

void UtilitiesCore::CClockOGL::Hold(Int32 numberOfFramesPerSecond)
{
	// if there is a limitation of frames per second, then we must:
	if (numberOfFramesPerSecond > 0)
	{
		// how many miliseconds we have with this frame rate?
		const Float miliPerFrame = 1.F / numberOfFramesPerSecond;
        Float delta = 0.f;
		// then we wait in this while until the elapsed time is greater than miliPerFrame
		while ((delta = (getTimeNow() - mStartFrameTime)) < miliPerFrame) { /* remains in hold */ } 
        mFps.Add(delta); // count delta time for fps calculation
	}
}

Float UtilitiesCore::CClockOGL::getTimeNow() const
{
	return ((Float)glutGet(GLUT_ELAPSED_TIME)) * 0.001F;
}
