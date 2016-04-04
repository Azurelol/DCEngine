/******************************************************************************/
/*!
@file   PhysicsCasting.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/4/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
@note References:
*/
/******************************************************************************/
#include "PhysicsCasting.h"

#include "Physics.h"
#include "../../Components/Transform.h"

namespace DCEngine {

  ZilchDefineType(CastFilter, "CastFilter", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, CastFilter, ZilchNoNames);
    ZilchBindField(builder, type, &CastFilter::CollisionGroup, "CollisionGroup", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastFilter::CollisionGroups, "CollisionGroups", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastFilter::Include, "Include", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastFilter::IgnoreStatic, "IgnoreStatic", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastFilter::IgnoreDynamic, "IgnoreDynamic", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastFilter::IgnoreGhost, "IgnoreGhost", Zilch::PropertyBinding::GetSet);
  }

  ZilchDefineType(CastResult, "CastResult", DCEngineCore, builder, type) {
    // DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(CastResult);
    ZilchBindField(builder, type, &CastResult::Distance, "Distance", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastResult::BodySpacePosition, "CastResult", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastResult::Normal, "Normal", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastResult::ObjectHit, "ObjectHit", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CastResult::WorldPosition, "WorldPosition", Zilch::PropertyBinding::GetSet);
  }

  ZilchDefineType(CastResultsRange, "CastResultsRange", DCEngineCore, builder, type) {
    ZilchBindDestructor(builder, type, CastResultsRange);
    // Interface
    builder.AddBoundProperty(type, "All", type, nullptr, &CastResultsRange::All, Zilch::MemberOptions::None);
    builder.AddBoundFunction(type, "MoveNext", &CastResultsRange::MoveNext, Zilch::ParameterArray(), ZilchTypeId(void), Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "Current", ZilchTypeId(Entity), nullptr, &CastResultsRange::Current, Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "IsNotEmpty", ZilchTypeId(Zilch::Boolean), nullptr, &CastResultsRange::IsNotEmpty, Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "Count", ZilchTypeId(Zilch::DoubleInteger), nullptr, &CastResultsRange::Count, Zilch::MemberOptions::None);
  }


  /**************************************************************************/
  /*!
  @brief Returns the container itself.
  */
  /**************************************************************************/
  void CastResultsRange::All(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    CastResultsRange* self = reinterpret_cast<CastResultsRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    call.Set(Zilch::Call::Return, *self);
  }

  /**************************************************************************/
  /*!
  @brief Moves the current iterator forward.
  */
  /**************************************************************************/
  void CastResultsRange::MoveNext(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    CastResultsRange* self = reinterpret_cast<CastResultsRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    ++(self->Begin);
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the current object.
  */
  /**************************************************************************/
  void CastResultsRange::Current(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    CastResultsRange* self = reinterpret_cast<CastResultsRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    auto toReturn = *(self->Begin);
    call.Set(Zilch::Call::Return, toReturn);
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the current object.
  */
  /**************************************************************************/
  void CastResultsRange::IsNotEmpty(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    CastResultsRange* self = reinterpret_cast<CastResultsRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    bool toReturn = self->sizeRange() == 0 ? false : true;
    call.Set(Zilch::Call::Return, toReturn);
  }

  /**************************************************************************/
  /*!
  @brief Returns the number of current objects in the range.
  */
  /**************************************************************************/
  void CastResultsRange::Count(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    CastResultsRange* self = reinterpret_cast<CastResultsRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    auto size = self->sizeRange();
    call.Set(Zilch::Call::Return, size);
  }

}