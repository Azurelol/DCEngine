/******************************************************************************/
/*!
@file   PhysicsCasting.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/26/2016
@brief  Physics casting can be used to check for the existence of objects based
on where they are in a Space. The most commonly-used form of physics
casting is ray casting, which checks for colliders along a ray, but
there are other varieties that check for colliders within a volume,
and so on.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
@note References:
      http://zero.digipen.edu/ZeroManual/Physics/PhysicsCasting/PhysicsCastingOverview.html
      #manualphysicscastingoverview
*/
/******************************************************************************/
#include "Physics.h"
#pragma once

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Performs a ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult Physics::CastRay(Ray & ray)
    {
      return CastResult();
    }

    /**************************************************************************/
    /*!
    @brief Performs a filtered ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @param filter A filter...
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult Physics::CastRay(Ray & ray, CastFilter & filter)
    {
      return CastResult();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the ray cast.
    @param ray A reference to the ray.
    @param count The number of objects returned is based on the count passed in.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastRay(Ray & ray, unsigned count)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered ray cast.
    @param ray A reference to the ray.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastRay(Ray & ray, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the segment cast.
    @param start The starting point of the segment.
    @param end The ending point of the segment.
    @param count The number of objects returned is based on the count passed in.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastSegment(Vec3 & start, Vec3 & end, unsigned count)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered segment cast.
    @param start The starting point of the segment.
    @param end The ending point of the segment.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastSegment(Vec3 & start, Vec3 & end, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered Aabb cast.
    @param center The origin.
    @param size The dimensions of the shape.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastAabb(Vec3 & center, Vec3 & size, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered sphere cast.
    @param center The origin.
    @param radius The radius of the sphere.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastSphere(Vec3 & center, float radius, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Cast out a collider to see whats it's colliding with.
    @param offset How much to offset the collider from its current position.
    @param testCollider A reference to the collider.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange Physics::CastCollider(Vec3 & offset, Components::Collider & testCollider, CastFilter & filter)
    {
      return CastResultsRange();
    }

  }
}