#ifndef _Frustum_H_
#define _Frustum_H_

#include "CommonTypes.h"
#include "IvVector4.h"
#include "IvPlane.h"
using namespace Types;

class Frustum
{
    // We create an enum of the sides so we don't have to call each side 0 or 1.
    // This way it makes it more understandable and readable when dealing with frustum sides.
    enum eFrustumSide
    {
        eFrustumSide_First = 0,
        eFrustumSide_Right = eFrustumSide_First,		// The RIGHT side of the frustum
        eFrustumSide_Left,
        eFrustumSide_Bottom,
        eFrustumSide_Top, 
        eFrustumSide_Back,
        eFrustumSide_Front,
        eFrustumSide_Total
    };

    //IvVector4 m_Frustum[eFrustumSide_Total];
    IvPlane m_frustum[eFrustumSide_Total];
public:
    void CalculateFrustum();
    bool IsSphereInFrustum(const IvVector3& point, float radius);
    bool IsPointInFrustum(const IvVector3& point);
    bool IsCubeInFrustum(const IvVector3& min, const IvVector3& max);
    bool IsAABBInFrustum(const IvAABB& aabb);
};

#endif //_Frustum_H_
