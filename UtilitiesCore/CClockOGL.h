//===============================================================================
// @ IvDebugger.h
// 
// Concrete implementation for OpenGL of IClock
//===============================================================================

#ifndef _CCLOCKOGL_H_
#define _CCLOCKOGL_H_

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "IClock.h"

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

using namespace Types;

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

    class FPS
    {
        Int32 scSamples = 4;
        Int32 mSamples = 0;
        Float mAccumulatedTime = 0.f;
        Int32 mFramesPerSecond = 0;
    public:
        Int32 GetFramesPerSecond() const
        {
            return mFramesPerSecond;
        }
        void Add(Float delta)
        {
            ++mSamples;
            mAccumulatedTime += delta;
            if (mSamples > scSamples)
            {
                // finds the average frame rendering time
                // and then calculate how many times this "fits" in a second
                mFramesPerSecond = static_cast<Int32>(1.f / (mAccumulatedTime / mSamples));
                mAccumulatedTime = 0.f;
                mSamples = 0;
            }
        }
    };

	class CClockOGL : public IClock
	{
	public:
		// constructor
		CClockOGL();
		// virtual destructor
		virtual ~CClockOGL();

		virtual void StartFrame() override;
        virtual Int32 FramesPerSecond() const override;
		virtual void Hold(Int32 numberOfFramesPerSecond) override;

	private:
		Float getTimeNow() const;
		Float mStartFrameTime;        
        FPS mFps;
		// copy operations
		CClockOGL(const CClockOGL& other);
		CClockOGL& operator=(const CClockOGL& other);
	};
}
//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------


#endif // _CCLOCKOGL_H_
