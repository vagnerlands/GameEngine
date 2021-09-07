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

// Like above, instead of saying a number for the ABC and D of the plane, we
// want to be more descriptive.
enum PlaneData
{
    A = 0,				// The X value of the plane's normal
    B = 1,				// The Y value of the plane's normal
    C = 2,				// The Z value of the plane's normal
    D = 3				// The distance the plane is from the origin
};

///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This normalizes a plane (A side) from a given frustum.
/////
///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void NormalizePlane(float frustum[6][4], int side)
{
    // Here we calculate the magnitude of the normal to the plane (point A B C)
    // Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
    // To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
    float magnitude = (float)sqrt(frustum[side][A] * frustum[side][A] +
        frustum[side][B] * frustum[side][B] +
        frustum[side][C] * frustum[side][C]);

    // Then we divide the plane's values by it's magnitude.
    // This makes it easier to work with.
    frustum[side][A] /= magnitude;
    frustum[side][B] /= magnitude;
    frustum[side][C] /= magnitude;
    frustum[side][D] /= magnitude;
}


///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This extracts our frustum from the projection and modelview matrix.
/////
///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Frustum::CalculateFrustum()
{
    //float   proj[16];								// This will hold our projection matrix
    //float   modl[16];								// This will hold our modelview matrix
    //float   clip[16];								// This will hold the clipping planes

    IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
    IvMatrix44 viewWorldMatrix = Graphics::IRenderer::mRenderer->GetViewMatrix() * Graphics::IRenderer::mRenderer->GetWorldMatrix();

    IvMatrix44 clipPlanes = projMatrix * viewWorldMatrix;

    //// glGetFloatv() is used to extract information about our OpenGL world.
    //// Below, we pass in GL_PROJECTION_MATRIX to abstract our projection matrix.
    //// It then stores the matrix into an array of [16].
    //glGetFloatv(GL_PROJECTION_MATRIX, proj);

    //// By passing in GL_MODELVIEW_MATRIX, we can abstract our model view matrix.
    //// This also stores it in an array of [16].
    //glGetFloatv(GL_MODELVIEW_MATRIX, modl);

    //// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
    //// it will give us our clipping planes.  To combine 2 matrices, we multiply them.

    //clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
    //clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
    //clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
    //clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

    //clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
    //clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
    //clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
    //clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

    //clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
    //clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
    //clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
    //clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

    //clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
    //clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
    //clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
    //clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

    //
    IvVector4 columns[4];
    clipPlanes.GetRows(columns[0], columns[1], columns[2], columns[3]);
    //clipPlanes.GetRows(rows[0], rows[1], rows[2], rows[3]);
    const float* fClip = clipPlanes.GetFloatPtr();
    //m_frustum[eFrustumSide_Right] = IvPlane(clipPlanes.GetColumns)

    // Now we actually want to get the sides of the frustum.  To do this we take
    // the clipping planes we received above and extract the sides from them.

    // This will extract the RIGHT side of the frustum
    //m_Frustum[eFrustumSide_Right] = IvVector4(
    //    fClip[3] - fClip[0], 
    //    fClip[7] - fClip[4], 
    //    fClip[11] - fClip[8], 
    //    fClip[15] - fClip[12]
    //);

    IvVector4 right = IvVector4(-columns[0].GetX(), -columns[0].GetY(), -columns[0].GetZ(), -columns[0].GetW()) + columns[3] ;
    m_frustum[eFrustumSide_Right] = IvPlane(right.GetX(), right.GetY(), right.GetZ(), right.GetW());
    /*m_frustum[eFrustumSide_Right] = IvPlane(
        fClip[3] - fClip[0],
        fClip[7] - fClip[4],
        fClip[11] - fClip[8],
        fClip[15] - fClip[12]
    );*/

    // Now that we have a normal (A,B,C) and a distance (D) to the plane,
    // we want to normalize that normal and distance.

    // Normalize the RIGHT side
    //NormalizePlane(m_Frustum, RIGHT);

    // This will extract the LEFT side of the frustum
    //m_Frustum[eFrustumSide_Left] = IvVector4(
    //    fClip[3] + fClip[0],
    //    fClip[7] + fClip[4],
    //    fClip[11] + fClip[8],
    //    fClip[15] + fClip[12]
    //);

    IvVector4 left = columns[0] + columns[3];
    m_frustum[eFrustumSide_Left] = IvPlane(left.GetX(), left.GetY(), left.GetZ(), left.GetW());
    //m_frustum[eFrustumSide_Left] = IvPlane(
    //    fClip[3] + fClip[0],
    //    fClip[7] + fClip[4],
    //    fClip[11] + fClip[8],
    //    fClip[15] + fClip[12]
    //);
    //m_Frustum[LEFT][A] = clip[3] + clip[0];
    //m_Frustum[LEFT][B] = clip[7] + clip[4];
    //m_Frustum[LEFT][C] = clip[11] + clip[8];
    //m_Frustum[LEFT][D] = clip[15] + clip[12];

    // Normalize the LEFT side
    //NormalizePlane(m_Frustum, LEFT);

    // This will extract the BOTTOM side of the frustum
    //m_Frustum[eFrustumSide_Bottom] = IvVector4(
    //    fClip[3] + fClip[1],
    //    fClip[7] + fClip[5],
    //    fClip[11] + fClip[9],
    //    fClip[15] + fClip[13]
    //);
    IvVector4 bottom = columns[1] + columns[3];
    m_frustum[eFrustumSide_Bottom] = IvPlane(bottom.GetX(), bottom.GetY(), bottom.GetZ(), bottom.GetW());
    /*m_frustum[eFrustumSide_Bottom] = IvPlane(
        fClip[3] + fClip[1],
        fClip[7] + fClip[5],
        fClip[11] + fClip[9],
        fClip[15] + fClip[13]
    );*/
    //m_Frustum[BOTTOM][A] = clip[3] + clip[1];
    //m_Frustum[BOTTOM][B] = clip[7] + clip[5];
    //m_Frustum[BOTTOM][C] = clip[11] + clip[9];
    //m_Frustum[BOTTOM][D] = clip[15] + clip[13];

    // Normalize the BOTTOM side
    //NormalizePlane(m_Frustum, BOTTOM);

    // This will extract the TOP side of the frustum
    //m_Frustum[eFrustumSide_Top] = IvVector4(
    //    fClip[3] - fClip[1],
    //    fClip[7] - fClip[5],
    //    fClip[11] - fClip[9],
    //    fClip[15] - fClip[13]
    //);
    //m_frustum[eFrustumSide_Top] = IvPlane(
    //    fClip[3] - fClip[1],
    //    fClip[7] - fClip[5],
    //    fClip[11] - fClip[9],
    //    fClip[15] - fClip[13]
    //);
    IvVector4 top = IvVector4(-columns[1].GetX(), -columns[1].GetY(), -columns[1].GetZ(), -columns[1].GetW()) + columns[3];
    m_frustum[eFrustumSide_Top] = IvPlane(top.GetX(), top.GetY(), top.GetZ(), top.GetW());
    //m_Frustum[TOP][A] = clip[3] - clip[1];
    //m_Frustum[TOP][B] = clip[7] - clip[5];
    //m_Frustum[TOP][C] = clip[11] - clip[9];
    //m_Frustum[TOP][D] = clip[15] - clip[13];

    // Normalize the TOP side
    //NormalizePlane(m_Frustum, TOP);

    // This will extract the BACK side of the frustum
    //m_Frustum[eFrustumSide_Back] = IvVector4(
    //    fClip[3] - fClip[2],
    //    fClip[7] - fClip[6],
    //    fClip[11] - fClip[10],
    //    fClip[15] - fClip[14]
    //);
    //m_frustum[eFrustumSide_Back] = IvPlane(
    //    fClip[3] - fClip[2],
    //    fClip[7] - fClip[6],
    //    fClip[11] - fClip[10],
    //    fClip[15] - fClip[14]
    //);
    IvVector4 back = columns[2] + columns[3] ;
    m_frustum[eFrustumSide_Back] = IvPlane(back.GetX(), back.GetY(), back.GetZ(), back.GetW());
    //m_Frustum[BACK][A] = clip[3] - clip[2];
    //m_Frustum[BACK][B] = clip[7] - clip[6];
    //m_Frustum[BACK][C] = clip[11] - clip[10];
    //m_Frustum[BACK][D] = clip[15] - clip[14];

    // Normalize the BACK side
    //NormalizePlane(m_Frustum, BACK);

    // This will extract the FRONT side of the frustum
    //m_Frustum[eFrustumSide_Front] = IvVector4(
    //    fClip[3] + fClip[2],
    //    fClip[7] + fClip[6],
    //    fClip[11] + fClip[10],
    //    fClip[15] + fClip[14]
    //);
    //m_frustum[eFrustumSide_Front] = IvPlane(
    //    fClip[3] + fClip[2],
    //    fClip[7] + fClip[6],
    //    fClip[11] + fClip[10],
    //    fClip[15] + fClip[14]
    //);
    IvVector4 _far = IvVector4(-columns[2].GetX(), -columns[2].GetY(), -columns[2].GetZ(), -columns[2].GetW()) + columns[3];
    m_frustum[eFrustumSide_Front] = IvPlane(_far.GetX(), _far.GetY(), _far.GetZ(), _far.GetW());
    //m_Frustum[FRONT][A] = clip[3] + clip[2];
    //m_Frustum[FRONT][B] = clip[7] + clip[6];
    //m_Frustum[FRONT][C] = clip[11] + clip[10];
    //m_Frustum[FRONT][D] = clip[15] + clip[14];

    // Normalize the FRONT side
    //NormalizePlane(m_Frustum, FRONT);
    //for (Int32 i = eFrustumSide_First; i < eFrustumSide_Total; i++)
    //{
    //    m_Frustum[i].Normalize();
    //}
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
bool Frustum::IsRectangleInFrustum(const IvVector3& min, const IvVector3& max)
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
