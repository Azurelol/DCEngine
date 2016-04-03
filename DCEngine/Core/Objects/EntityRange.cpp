/*****************************************************************************/
/*!
@file   EntityRange.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/2/2016
@brief  Custom container for Entities, used by Zilch.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Entity.h"

namespace DCEngine {

  ZilchDefineType(EntityRange, "EntityRange", DCEngineCore, builder, type) {    
    
    //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    //type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    //ZilchBindConstructor(builder, type, EntityRange, ZilchNoNames);
    ZilchBindDestructor(builder, type, EntityRange);
    // Interface
    builder.AddBoundProperty(type, "All", type, nullptr, &EntityRange::All, Zilch::MemberOptions::None);
    builder.AddBoundFunction(type, "MoveNext", &EntityRange::MoveNext, Zilch::ParameterArray(), ZilchTypeId(void), Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "Current", ZilchTypeId(Entity), nullptr, &EntityRange::Current, Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "IsNotEmpty", ZilchTypeId(Zilch::Boolean), nullptr, &EntityRange::IsNotEmpty, Zilch::MemberOptions::None);
    builder.AddBoundProperty(type, "Count", ZilchTypeId(Zilch::DoubleInteger), nullptr, &EntityRange::Count, Zilch::MemberOptions::None);
  }

  /**************************************************************************/
  /*!
  @brief Returns the container itself.
  */
  /**************************************************************************/
  void EntityRange::All(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    EntityRange* self = reinterpret_cast<EntityRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    call.Set(Zilch::Call::Return, *self);
  }

  /**************************************************************************/
  /*!
  @brief Moves the current iterator forward.
  */
  /**************************************************************************/
  void EntityRange::MoveNext(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    EntityRange* self = reinterpret_cast<EntityRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    ++(self->Begin);
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the current object.
  */
  /**************************************************************************/
  void EntityRange::Current(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    EntityRange* self = reinterpret_cast<EntityRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    auto toReturn = *(self->Begin);
    call.Set(Zilch::Call::Return, toReturn);
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the current object.
  */
  /**************************************************************************/
  void EntityRange::IsNotEmpty(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    EntityRange* self = reinterpret_cast<EntityRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    bool toReturn = self->sizeRange() == 0 ? false : true;
    call.Set(Zilch::Call::Return, toReturn);
  }

  /**************************************************************************/
  /*!
  @brief Returns the number of current objects in the range.
  */
  /**************************************************************************/
  void EntityRange::Count(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    EntityRange* self = reinterpret_cast<EntityRange*>(call.GetHandle(Zilch::Call::This).Dereference());
    auto size = self->sizeRange();
    call.Set(Zilch::Call::Return, size);
  }

}