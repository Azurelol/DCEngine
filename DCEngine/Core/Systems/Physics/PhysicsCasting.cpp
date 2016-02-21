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
#pragma once
#include "Physics.h"
#include "../../Components/Transform.h"
namespace DCEngine
{
  namespace Systems 
  {

    Vec3 GenerateSegment(Ray &ray, Components::PhysicsSpace *Space)
    {
      
      Vec3 Sstart = Vec3(Space->MinX, Space->MaxY, 0);
      Vec3 Send = Vec3(Space->MaxX, Space->MaxY, 0);

      Vec3 SegmentDir = glm::normalize(Send - Sstart);

      float T1, T2;
      T1 = (-ray.Direction.y * (ray.Origin.x - Sstart.x) + ray.Direction.x * (ray.Origin.y - Sstart.y)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);
      T2 = (SegmentDir.x * (ray.Origin.y - Sstart.y) - SegmentDir.y * (ray.Origin.x - Sstart.x)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);

      if (T1 >= 0 && T1 <= 1 && T2 >= 0 && T2 <= 1)
      {
        // Collision detected
        // Return the point of intersection
        return Vec3(ray.Origin + (T2 * ray.Direction));
      }


      Sstart = Vec3(Space->MaxX, Space->MaxY, 0);
      Send = Vec3(Space->MaxX, Space->MinY, 0);

      SegmentDir = glm::normalize(Send - Sstart);

      T1 = (-ray.Direction.y * (ray.Origin.x - Sstart.x) + ray.Direction.x * (ray.Origin.y - Sstart.y)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);
      T2 = (SegmentDir.x * (ray.Origin.y - Sstart.y) - SegmentDir.y * (ray.Origin.x - Sstart.x)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);

      if (T1 >= 0 && T1 <= 1 && T2 >= 0 && T2 <= 1)
      {
        // Collision detected
        // Return the point of intersection
        return Vec3(ray.Origin + (T2 * ray.Direction));
      }

      Sstart = Vec3(Space->MaxX, Space->MinY, 0);
      Send = Vec3(Space->MinX, Space->MinY, 0);

      SegmentDir = glm::normalize(Send - Sstart);

      T1 = (-ray.Direction.y * (ray.Origin.x - Sstart.x) + ray.Direction.x * (ray.Origin.y - Sstart.y)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);
      T2 = (SegmentDir.x * (ray.Origin.y - Sstart.y) - SegmentDir.y * (ray.Origin.x - Sstart.x)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);

      if (T1 >= 0 && T1 <= 1 && T2 >= 0 && T2 <= 1)
      {
        // Collision detected
        // Return the point of intersection
        return Vec3(ray.Origin + (T2 * ray.Direction));
      }

      Sstart = Vec3(Space->MinX, Space->MinY, 0);
      Send = Vec3(Space->MinX, Space->MaxY, 0);

      SegmentDir = glm::normalize(Send - Sstart);

      T1 = (-ray.Direction.y * (ray.Origin.x - Sstart.x) + ray.Direction.x * (ray.Origin.y - Sstart.y)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);
      T2 = (SegmentDir.x * (ray.Origin.y - Sstart.y) - SegmentDir.y * (ray.Origin.x - Sstart.x)) / (-SegmentDir.x * ray.Direction.y + ray.Direction.x * SegmentDir.y);

      if (T1 >= 0 && T1 <= 1 && T2 >= 0 && T2 <= 1)
      {
        // Collision detected
        // Return the point of intersection
        return Vec3(ray.Origin + (T2 * ray.Direction));
      }



      // if we get here there is a serious problem
      throw (DCException("shit man this is bad my line segment algorithm dont work"));

      return Vec3();
    }

    bool isGroup(std::vector<DCEngine::CollisionGroup> &Groups, DCEngine::CollisionGroup group)
    {
      for (auto Group : Groups)
      {
        if (Group.Name() == group.Name())
        {
          return true;
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief Performs a ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult Physics::CastRay(Ray & ray, Components::PhysicsSpace *Space)
    {
      CastResult retval;

      Vec3 endpoint = GenerateSegment(ray, Space);

      std::pair<Vec3, Vec3> segment;
      segment.first = ray.Origin;
      segment.second = endpoint;
      
      retval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::SegmentToCollider(segment, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (Distance < retval.Distance)
          {
            retval.Distance = Distance;
            Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

            retval.WorldPosition = transform->Translation;

          }
        }

      }

      return retval;
    }

    /**************************************************************************/
    /*!
    @brief Performs a filtered ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @param filter A filter...
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult Physics::CastRay(Ray & ray, CastFilter & filter, Components::PhysicsSpace *Space)
    {
      CastResult retval;

      Vec3 endpoint = GenerateSegment(ray, Space);

      std::pair<Vec3, Vec3> segment;
      segment.first = ray.Origin;
      segment.second = endpoint;

      retval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::SegmentToCollider(segment, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (filter.Include)
          {
            if (Distance < retval.Distance && isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              retval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              retval.WorldPosition = transform->Translation;

            }
          }
          else
          {
            if (Distance < retval.Distance && !isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              retval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              retval.WorldPosition = transform->Translation;

            }
          }
        }

      }

      return retval;
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