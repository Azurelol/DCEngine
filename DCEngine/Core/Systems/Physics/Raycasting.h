/******************************************************************************/
/*!
@file   Ray.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/26/2016
@brief  A ray is a linear arrangement of points that starts at a given location
and extends infinitely far in a given direction. It is also known as a
half-line.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

// PhysicsCasting objects
#include "Ray.h"
#include "../../DaisyVector.h"

namespace DCEngine {

  // Forward declarations
  //class Collider;
  //class GameObject;
  class CollisionGroup;

  /**************************************************************************/
  /*!
  @class CastResult is a class that contains the results of a cast on a
  particular object, relative to where the ray's origin.
  */
  /**************************************************************************/
  struct CastResult {
#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareBaseType(CastResult, Zilch::TypeCopyMode::ReferenceType);
#endif
    //Collider* Collider;
    float Distance;
    // the position of the ray-line segment collision in world coordinates
    Vec3 BodySpacePosition;
    Vec3 Normal;
    GameObject* ObjectHit = NULL;
    Vec3 WorldPosition;
  };

  // A container of Cast Results
  using CastResultsRange = std::vector<CastResult>;

  /**************************************************************************/
  /*!
  @class CastFilter is a class that allows the client to filter out specific
  objects from a raycasting. Most often used to ignore collision groups,
  or certain colliders.
  */
  /**************************************************************************/
  struct CastFilter {
#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareBaseType(CastFilter, Zilch::TypeCopyMode::ReferenceType);
#endif
    bool Include = false;
    std::vector<CollisionGroup> CollisionGroups;
    bool IgnoreStatic = false;
    bool IgnoreDynamic = false;
    bool IgnoreGhost = false;    
  };



}