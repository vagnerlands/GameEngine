//===============================================================================
// @ AABB.h
// 
// Description
// ------------------------------------------------------------------------------
//
//===============================================================================

#ifndef _AABB_H_
#define _AABB_H_

#include "CommonTypes.h"
#include "IvVector3.h"

// axis aligned bounding box
struct AABB
{
    AABB() : Min{ 0.f,0.f,0.f }, Max{ 0.f,0.f,0.f } {}
    IvVector3 Min;
    IvVector3 Max;
};

#endif //_AABB_H_
