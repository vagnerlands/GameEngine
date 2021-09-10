#include "Frustum.h"
#include "IRenderer.h"
#include "OGLTypes.h"
#include "glm/glm.hpp"

//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		digiben@gametutorials.com			 //
//																		 //
//		$Program:		Frustum Culling									 //
//																		 //
//		$Description:	Demonstrates checking if shapes are in view		 //
//																		 //
//***********************************************************************//

///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This extracts our frustum from the projection and modelview matrix.
/////
///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Frustum::CalculateFrustum()
{
    IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
    IvMatrix44 viewWorldMatrix = Graphics::IRenderer::mRenderer->GetViewMatrix() * Graphics::IRenderer::mRenderer->GetWorldMatrix();

    IvMatrix44 clipPlanes = projMatrix * viewWorldMatrix;
    //
    IvVector4 columns[4];
    clipPlanes.GetRows(columns[0], columns[1], columns[2], columns[3]);

    IvVector4 right = -columns[0] + columns[3]; 
    m_frustum[eFrustumSide_Right] = IvPlane(right.GetX(), right.GetY(), right.GetZ(), right.GetW());

    IvVector4 left = columns[0] + columns[3];
    m_frustum[eFrustumSide_Left] = IvPlane(left.GetX(), left.GetY(), left.GetZ(), left.GetW());

    IvVector4 bottom = columns[1] + columns[3];
    m_frustum[eFrustumSide_Bottom] = IvPlane(bottom.GetX(), bottom.GetY(), bottom.GetZ(), bottom.GetW());

    IvVector4 top = -columns[1] + columns[3];
    m_frustum[eFrustumSide_Top] = IvPlane(top.GetX(), top.GetY(), top.GetZ(), top.GetW());

    IvVector4 back = columns[2] + columns[3] ;
    m_frustum[eFrustumSide_Back] = IvPlane(back.GetX(), back.GetY(), back.GetZ(), back.GetW());

    IvVector4 _far = -columns[2] + columns[3];
    m_frustum[eFrustumSide_Front] = IvPlane(_far.GetX(), _far.GetY(), _far.GetZ(), _far.GetW());
}


/////////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////	This determines if a sphere is inside of our frustum by it's center and radius.
///////
/////////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool Frustum::IsSphereInFrustum(const IvVector3& point, float radius)
{
    // Now this function is almost identical to the PointInFrustum(), except we
    // now have to deal with a radius around the point.  The point is the center of
    // the radius.  So, the point might be outside of the frustum, but it doesn't
    // mean that the rest of the sphere is.  It could be half and half.  So instead of
    // checking if it's less than 0, we need to add on the radius to that.  Say the
    // equation produced -2, which means the center of the sphere is the distance of
    // 2 behind the plane.  Well, what if the radius was 5?  The sphere is still inside,
    // so we would say, if(-2 < -5) then we are outside.  In that case it's false,
    // so we are inside of the frustum, but a distance of 3.  This is reflected below.

    // Go through all the sides of the frustum
    for (Int32 i = eFrustumSide_First; i < eFrustumSide_Total; i++)
    {
        if (m_frustum[i].Test(point) < -radius)
            return false;
    }

    // The sphere was inside of the frustum!
    return true;
}


/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////	This determines if a cube is in or around our frustum by it's center and 1/2 it's length
///////
/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
bool Frustum::IsPointInFrustum(const IvVector3& point)
{
    for (Int32 i = eFrustumSide_First; i < eFrustumSide_Front; i++)
    {
        if (m_frustum[i].Test(point) < 0.f)
            return false;
    }

    return true;
}

/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////	This determines if a rectangle by naive approach (best approach is to check the 4 corners)
///////
/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
bool Frustum::IsCubeInFrustum(const IvVector3& min, const IvVector3& max)
{
    for (Int32 i = eFrustumSide_First; i < eFrustumSide_Total; i++)
    {
        if (m_frustum[i].Test(min) < 0.f)
            return false;
        if (m_frustum[i].Test(max) < 0.f)
            return false;
    }

    return true;
}

bool Frustum::IsAABBInFrustum(const IvAABB & aabb)
{
    // first, check if at least one point of the AABB is in the frustum
    if (IsPointInFrustum(aabb.GetMinima())
        || IsPointInFrustum(aabb.GetMaxima()))
    {
        return true;
    }
    else
    {
        // if no point is inside, there is a change the AABB is intersecting the frustum, so we test the aabb against all planes
        for (Int32 i = eFrustumSide_First; i < eFrustumSide_Total; i++)
        {       
            // we'll check if 
            if (aabb.Classify(m_frustum[i]) == IvAABB::eRelation_HasIntersection)
                return true; 
        }
    }

    return false;
}

//
//
///////////////////////////////////////////////////////////////////////////////////
////
//// * QUICK NOTES * 
////
//// WOZZERS!  That seemed like an incredible amount to look at, but if you break it
//// down, it's not.  Frustum culling is a VERY useful thing when it comes to 3D.
//// If you want a large world, there is no way you are going to send it down the
//// 3D pipeline every frame and let OpenGL take care of it for you.  That would
//// give you a 0.001 frame rate.  If you hit '+' and bring the sphere count up to
//// 1000, then take off culling, you will see the HUGE difference it makes.  
//// Also, you wouldn't really be rendering 1000 spheres.  You would most likely
//// use the sphere code for larger objects.  Let me explain.  Say you have a bunch
//// of objects, well... all you need to do is give the objects a radius, and then
//// test that radius against the frustum.  If that sphere is in the frustum, then you
//// render that object.  Also, you won't be rendering a high poly sphere so it won't
//// be so slow.  This goes for bounding box's too (Cubes).  If you don't want to
//// do a cube, it is really easy to convert the code for rectangles.  Just pass in
//// a width and height, instead of just a length.  Remember, it's HALF the length of
//// the cube, not the full length.  So it would be half the width and height for a rect.
//// 
//// This is a perfect starter for an octree tutorial.  Wrap you head around the concepts
//// here and then see if you can apply this to making an octree.  Hopefully we will have
//// a tutorial up and running for this subject soon.  Once you have frustum culling,
//// the next step is getting space partitioning.  Either it being a BSP tree of an Octree.
//// 
//// Let's go over a brief overview of the things we learned here:
////
//// 1) First we need to abstract the frustum from OpenGL.  To do that we need the
////    projection and modelview matrix.  To get the projection matrix we use:
////
////			glGetFloatv( GL_PROJECTION_MATRIX, /* An Array of 16 floats */ );
////    Then, to get the modelview matrix we use:
////
////			glGetFloatv( GL_MODELVIEW_MATRIX, /* An Array of 16 floats */ );
////    
////	  These 2 functions gives us an array of 16 floats (The matrix).
////
//// 2) Next, we need to combine these 2 matrices.  We do that by matrix multiplication.
////
//// 3) Now that we have the 2 matrixes combined, we can abstract the sides of the frustum.
////    This will give us the normal and the distance from the plane to the origin (ABC and D).
////
//// 4) After abstracting a side, we want to normalize the plane data.  (A B C and D).
////
//// 5) Now we have our frustum, and we can check points against it using the plane equation.
////    Once again, the plane equation (A*x + B*y + C*z + D = 0) says that if, point (X,Y,Z)
////    times the normal of the plane (A,B,C), plus the distance of the plane from origin,
////    will equal 0 if the point (X, Y, Z) lies on that plane.  If it is behind the plane
////    it will be a negative distance, if it's in front of the plane (the way the normal is facing)
////    it will be a positive number.
////
////
//// If you need more help on the plane equation and why this works, download our
//// Ray Plane Intersection Tutorial at www.GameTutorials.com.
////
//// That's pretty much it with frustums.  There is a lot more we could talk about, but
//// I don't want to complicate this tutorial more than I already have.
////
//// I want to thank Mark Morley for his tutorial on frustum culling.  Most of everything I got
//// here comes from his teaching.  If you want more in-depth, visit his tutorial at:
////
//// http://www.markmorley.com/opengl/frustumculling.html
////
//// Good luck!
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
////
////
