/******************************************************************************/
/*!
@file   PhysicsCasting.cpp
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
  CastFilter *MakeCastFilter(void);

  ZilchDefineType(CollisionVector, "CollisionVector", DCEngineCore, builder, type) {
    // DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(CastResultsRange);
    ZilchBindMethod(builder, type, &CollisionVector::size, ZilchNoOverload, "size", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::capacity, ZilchNoOverload, "capacity", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::empty, ZilchNoOverload, "empty", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::reserve, ZilchNoOverload, "reserve", "n");
    ZilchBindMethod(builder, type, &CollisionVector::shrink_to_fit, ZilchNoOverload, "shrink_to_fit", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::at, ZilchNoOverload, "at", "n");
    ZilchBindMethod(builder, type, &CollisionVector::front, ZilchNoOverload, "front", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::back, ZilchNoOverload, "back", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::push_back, ZilchNoOverload, "push_back", "val");
    ZilchBindMethod(builder, type, &CollisionVector::pop_back, ZilchNoOverload, "pop_back", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionVector::insert, ZilchNoOverload, "insert", "position, val");
    ZilchBindMethod(builder, type, &CollisionVector::clear, ZilchNoOverload, "clear", ZilchNoNames);

  }

  ZilchDefineType(Ray, "Ray", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, Ray, ZilchNoNames);
    //DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Ray);
    ZilchBindField(builder, type, &Ray::Origin, "Origin", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &Ray::Direction, "Direction", Zilch::PropertyBinding::GetSet);
  }


  CollisionVector& CollisionVector::operator= (const CollisionVector& ref)
  {
    list = ref.list;
    return *this;
  }

  unsigned CollisionVector::size()
  {
    return list.size();
  }
  unsigned CollisionVector::capacity() const
  {
    return list.capacity();
  }
  bool CollisionVector::empty() const
  {
    return list.empty();
  }
  void CollisionVector::reserve(unsigned n)
  {
    list.reserve(n);
  }
  void CollisionVector::shrink_to_fit()
  {
    list.shrink_to_fit();
  }
  CollisionGroup& CollisionVector::operator[] (unsigned n)
  {
    return *list[n];
  }
  CollisionGroup& CollisionVector::at(unsigned n)
  {
    return *list.at(n);
  }
  CollisionGroup& CollisionVector::front()
  {
    return *list.front();
  }
  CollisionGroup& CollisionVector::back()
  {
    return *list.back();
  }
  void CollisionVector::push_back(CollisionGroup* val)
  {
    list.push_back(val);
  }
  void CollisionVector::pop_back()
  {
    list.pop_back();
  }
  void CollisionVector::insert(unsigned position, CollisionGroup* val)
  {
    list[position] = val;
  }
  void CollisionVector::clear()
  {
    list.clear();
  }

  /**************************************************************************/
  /*!
  @brief help function for zilch to make a cast filter.
  */
  /**************************************************************************/
  CastFilter *MakeCastFilter(void)
  {
    return new CastFilter;
  }

  namespace Systems 
  {

    bool isGroup(CollisionVector &Groups, const DCEngine::CollisionGroup & group)
    {
      for (int i = 0; Groups.at(i).Name() != Groups.back().Name(); i++)
      {
        auto Group = Groups.at(i);
        if (Group.Name() == group.Name())
        {
          return true;
        }
      }
      if (Groups.back().Name() == group.Name())
      {
        return true;
      }
      return false;
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
      
      retval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::RayToCollider(ray, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (Distance < retval.Distance)
          {
            retval.Distance = Distance;
            Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

            retval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
            retval.WorldPosition = transform->Translation;
            retval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
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
      // Hack: Adding the single collision group to
      if (filter.CollisionGroup != nullptr)
      {
        filter.CollisionGroups.push_back((filter.CollisionGroup));
      }

      CastResult retval;

      retval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::RayToCollider(ray, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (filter.Include)
          {
            if (Distance < retval.Distance && isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              retval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              retval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
              retval.WorldPosition = transform->Translation;
              retval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
            }
          }
          else
          {
            if (Distance < retval.Distance && !isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              retval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              retval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
              retval.WorldPosition = transform->Translation;
              retval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
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
    CastResultsRange Physics::CastRay(Ray & ray, unsigned count, Components::PhysicsSpace *Space)
    {
      CastResultsRange retval;
      
      CastResult pushval;

      pushval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::RayToCollider(ray, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (Distance < FLT_MAX)
          {
            pushval.Distance = Distance;
            Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

            pushval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
            pushval.WorldPosition = transform->Translation;
            pushval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
            retval.push_back(pushval);
          }
        }

      }

      return retval;
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
    CastResultsRange Physics::CastRay(Ray & ray, unsigned count, CastFilter & filter, Components::PhysicsSpace *Space)
    {
      CastResultsRange retval;

      CastResult pushval;

      pushval.Distance = FLT_MAX;

      auto colliders = Space->AllColliders();

      float Distance = 0;

      for (auto &object : colliders)
      {

        if (Collision::RayToCollider(ray, dynamic_cast<GameObject*>(object->Owner()), Distance))
        {
          if (filter.Include)
          {
            if (Distance < FLT_MAX && isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              pushval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              pushval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
              pushval.WorldPosition = transform->Translation;
              pushval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
              retval.push_back(pushval);
            }
          }
          else
          {
            if (Distance < FLT_MAX && !isGroup(filter.CollisionGroups, object->getCollisionGroup()))
            {
              pushval.Distance = Distance;
              Components::Transform* transform = object->Owner()->getComponent<Components::Transform>();

              pushval.BodySpacePosition = ray.Origin + glm::normalize(ray.Direction) * Distance;
              pushval.WorldPosition = transform->Translation;
              pushval.ObjectHit = dynamic_cast<GameObject*>(object->Owner());
              retval.push_back(pushval);
            }
          }
        }

      }

      return retval;
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
    CastResultsRange Physics::CastSegment(Vec3 & start, Vec3 & end, unsigned count, Components::PhysicsSpace *Space)
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
    CastResultsRange Physics::CastSegment(Vec3 & start, Vec3 & end, unsigned count, CastFilter & filter, Components::PhysicsSpace *Space)
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
    CastResultsRange Physics::CastAabb(Vec3 & center, Vec3 & size, unsigned count, CastFilter & filter, Components::PhysicsSpace *Space)
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
    CastResultsRange Physics::CastSphere(Vec3 & center, float radius, unsigned count, CastFilter & filter, Components::PhysicsSpace *Space)
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
    CastResultsRange Physics::CastCollider(Vec3 & offset, Components::Collider & testCollider, CastFilter & filter, Components::PhysicsSpace *Space)
    {
      return CastResultsRange();
    }

  }
}