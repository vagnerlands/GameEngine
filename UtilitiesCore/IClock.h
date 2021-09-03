//===============================================================================
// @ IClock.h
// Holds time implementation such as:
// * current timestamp
// * elapsed time
// * 
//===============================================================================

#ifndef _ICLOCK_H_
#define _ICLOCK_H_

#include "CommonTypes.h"

using namespace Types;

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{
	class IClock
	{
	public:
		// constructor
		IClock();
		// virtual destructor
		virtual ~IClock();

        virtual Int32 FramesPerSecond() const = 0;
    
		virtual Float GetElapsedTime();

		virtual Float GetTimeInMili();

		virtual void StartFrame() = 0;

		virtual void Hold(Int32 numberOfFramesPerSecond) = 0;

	protected:
		Float mElapsedTime;
		Float mCurrentTime;
    
	private:
		// copy operations
		IClock( const IClock& other );
		IClock& operator=( const IClock& other );
	};


	//-------------------------------------------------------------------------------
	//-- Inlines --------------------------------------------------------------------
	//-------------------------------------------------------------------------------

	inline Float
		IClock::GetElapsedTime()
	{
		return mElapsedTime;
	}

	inline Float
		IClock::GetTimeInMili()
	{
		return mCurrentTime;
	}
}

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------


#endif // _ICLOCK_H_
