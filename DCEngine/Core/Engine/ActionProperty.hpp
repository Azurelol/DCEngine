/*****************************************************************************/
/*!
@file      ActionProperty.hpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      3/24/2016
@brief     ActionProperty template file.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ActionProperty.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief ActionZilchFloatProperty constructor.
  @param PropertyType The POD or class type of the property;
  @param set A reference to the set this property is part of.
  @param prop A The property delegate.
  @param value The new value to interpolate over the given duration.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/
  template<typename PropertyType>
  ActionZilchFloatProperty<PropertyType>::ActionZilchFloatProperty(ActionSetPtr set, 
                                                                   Zilch::PropertyDelegateTemplate deleg,
                                                                   PropertyType value, Real duration, 
                                                                   Ease ease)
    : ActionZilchProperty(set, deleg, duration, ease),
      EndValue(value)
  {
    if (DCE_TRACE_ACTIONS_CTOR)
      DCTrace << "ActionZilchFloatProperty::ActionZilchFloatProperty: Constructed! \n";
    // Get the initial value of the property
    Zilch::Call call(Property.Get, Systems::ZilchInterface::GetState());
    call.Invoke(Report);
    InitialValue = call.Get<PropertyType>(Zilch::Call::Return);
    // Calculate the difference
    Difference = EndValue - InitialValue;
  }

  /**************************************************************************/
  /*!
  @brief Interpolates the property towards the intended value.
  @param dt The delta time.
  */
  /**************************************************************************/
  template<typename PropertyType>
  inline float ActionZilchFloatProperty<PropertyType>::Interpolate(float dt)
  {
    if (Elapsed == 0)
    {
      Zilch::Call call(Property.Get, Systems::ZilchInterface::GetState());
      call.Invoke(Report);
      InitialValue = call.Get<PropertyType>(Zilch::Call::Return);
      Difference = EndValue - InitialValue;
    }

    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    if (DCE_TRACE_ACTIONS_UPDATE)
          DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    Zilch::Call call(Property.Set, Systems::ZilchInterface::GetState());

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    // If finished...
    if (timeLeft <= dt) {
      timeConsumed = dt;
      IsFinished = true;
      // Set the final value
      call.Set(0, EndValue);
      call.Invoke(Report);
    }
    else {
      timeConsumed = timeLeft;
      // Calculate the interpolated value
      auto updatedVal = InitialValue + Difference * Easing::Calculate((Elapsed / Duration), EaseType);
      call.Set(0, updatedVal);
      // Set the new value
      call.Invoke(Report);
    }    

    return timeConsumed;
  }

  /* TEMPLATE DEFINITION */
  /**************************************************************************/
  /*!
  @brief ActionProperty constructor.
  @param PropertyType The POD or class type of the property;
  @param set A reference to the set this property is part of.
  @param property A reference to the property to be modified.
  @param value The new value to interpolate over the given duration.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/
  template<typename PropertyType>
  ActionFloatProperty<PropertyType>::ActionFloatProperty(ActionSetPtr set, PropertyType & prop,
                                                         PropertyType value, Real duration, Ease ease)
    : ActionBaseProperty(set, duration, ease), Property(prop), InitialValue(prop), EndValue(value)
  {
    if (DCE_TRACE_ACTIONS_CTOR)
      DCTrace << "ActionProperty::ActionProperty: Constructed! \n";

    // Initialize the rest of the values
    Difference = EndValue - InitialValue;
  }

  /**************************************************************************/
  /*!
  @brief Updates the ActionProperty.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  float ActionFloatProperty<PropertyType>::Interpolate(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    // If finished...
    if (timeLeft <= dt) {
      timeConsumed = dt;
      IsFinished = true;
      // Set the final value
      Property = EndValue;
    }
    else {
      timeConsumed = timeLeft;
      // Set the new value
      Property = InitialValue + Difference * Easing::Calculate((Elapsed / Duration), EaseType);
    }

    return timeConsumed;
  }

  /**************************************************************************/
  /*!
  @brief ActionBooleanProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param prop A reference to the property.
  @param value The desired end value for the property.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  template<typename BooleanType>
  ActionBooleanProperty<BooleanType>::ActionBooleanProperty(ActionSetPtr set, BooleanType & prop, BooleanType value, Real duration, Ease ease)
    : ActionBaseProperty(set, duration, ease), Property(prop), EndValue(value)
  {
  }

  /**************************************************************************/
  /*!
  @brief Updates the ActionProperty.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename BooleanType>
  inline float ActionBooleanProperty<BooleanType>::Interpolate(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    if (timeLeft <= dt) {
      if (DCE_TRACE_ACTIONS_UPDATE)
        DCTrace << "ActionProperty::Update: Finished! \n";
      IsFinished = true;
      timeConsumed = dt;
      // Set the boolean value only at the end!
      Property = EndValue;
    }
    else
      timeConsumed = timeLeft;

    return timeConsumed;
  }


}