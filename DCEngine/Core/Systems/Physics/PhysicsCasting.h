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

namespace DCEngine {

  // Forward declarations
  class CollisionGroup;
  class GameObject;

  /**************************************************************************/
  /*!
  @class CollisionVector A vector of collisions.
  */
  /**************************************************************************/
  class CollisionVector
  {
    ZilchDeclareBaseType(CollisionVector, Zilch::TypeCopyMode::ReferenceType);
    std::vector<CollisionGroup*> list;
  public:
    CollisionVector& operator= (const CollisionVector& ref);
    unsigned size();
    unsigned capacity() const;
    bool empty() const;
    void reserve(unsigned n);
    void shrink_to_fit();
    CollisionGroup& operator[] (unsigned n);
    CollisionGroup& at(unsigned n);
    CollisionGroup& front();
    CollisionGroup& back();
    void push_back(CollisionGroup* val);
    void pop_back();
    void insert(unsigned position, CollisionGroup* val);
    void clear();
  };

  /**************************************************************************/
  /*!
  @class CastFilter is a class that allows the client to filter out specific
  objects from a raycasting. Most often used to ignore collision groups,
  or certain colliders.
  */
  /**************************************************************************/
  struct CastFilter {
    ZilchDeclareBaseType(CastFilter, Zilch::TypeCopyMode::ReferenceType);
    bool Include = false;
    CollisionGroup* CollisionGroup;
    CollisionVector CollisionGroups;
    bool IgnoreStatic = false;
    bool IgnoreDynamic = false;
    bool IgnoreGhost = false;
  };

  /**************************************************************************/
  /*!
  @class CastResult is a class that contains the results of a cast on a
  particular object, relative to where the ray's origin.
  */
  /**************************************************************************/
  struct CastResult {
    ZilchDeclareBaseType(CastResult, Zilch::TypeCopyMode::ReferenceType);
    //Collider* Collider;
    float Distance;
    // the position of the ray-line segment collision in world coordinates
    Vec3 BodySpacePosition;
    Vec3 Normal;
    GameObject* ObjectHit = NULL;
    Vec3 WorldPosition;
  };
  

  /**************************************************************************/
  /*!
  @class CastResultsRange A range-based container of CastResults.
  */
  /**************************************************************************/
  class CastResultsRange : public std::vector<CastResult>
  {
    using Iterator = CastResultsRange::iterator;
    Iterator Begin;
    Iterator End;
    bool isRange() { return Begin != End; }
    const Iterator beginRange() const { return Begin; }
    const Iterator endRange() const { return End; }
    Iterator beginRange() { return Begin; }
    Iterator endRange() { return End; }
    size_t sizeRange() const { return End - Begin; }

  public:
    CastResultsRange() : Begin(this->begin()), End(this->end()) {}
    ZilchDeclareBaseType(CastResultsRange, Zilch::TypeCopyMode::ValueType);

    static void All(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void MoveNext(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void Current(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void IsNotEmpty(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void Count(Zilch::Call& call, Zilch::ExceptionReport& report);

  };

}