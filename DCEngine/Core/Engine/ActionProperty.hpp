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
  ActionProperty<PropertyType>::ActionProperty(ActionSetPtr set, PropertyType & prop,
    PropertyType value, Real duration, Ease ease)
    : Property(prop), InitialValue(Property), EndValue(value), Duration(duration), Ease_(ease)
  {
    if (DCE_TRACE_ACTIONS_CTOR)
      DCTrace << "ActionProperty::ActionProperty: Constructed! \n";

    // Initialize the rest of the values
    Difference = EndValue - InitialValue;
  }

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
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    // Calculate the interpolated value
    auto updatedVal = InitialValue + Difference * Easing::Calculate((Elapsed / Duration), EaseType);
    // Set the new value
    Zilch::Call call(Property.Set, Systems::ZilchInterface::GetState());
    call.Set(0, updatedVal);
    call.Invoke(Report);

    if (Elapsed >= Duration) {
      if (DCE_TRACE_ACTIONS_UPDATE)
        DCTrace << "ActionProperty::Update: Finished! \n";
      IsFinished = true;
      // Set the value to the max
      call.Set(0, EndValue);
      call.Invoke(Report);
    }

    else if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    if (timeLeft < dt)
      timeConsumed = dt;
    else
      timeConsumed = timeLeft;

    return timeConsumed;
  }

  /**************************************************************************/
  /*!
  @brief ActionZilchFloatPropertyOld constructor.
  @param PropertyType The POD or class type of the property;
  @param set A reference to the set this property is part of.
  @param prop A The property delegate.
  @param value The new value to interpolate over the given duration.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/
  template<typename PropertyType>
  ActionZilchFloatPropertyOld<PropertyType>::ActionZilchFloatPropertyOld(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
    PropertyType value, Real duration, Ease ease)
    : Property(deleg), EndValue(value), Duration(duration), Ease_(ease)
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
  @brief Updates the ZilchActionProperty.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  float ActionZilchFloatPropertyOld<PropertyType>::Update(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    // Calculate the interpolated value
    auto updatedVal = InitialValue + Difference * Easing::Calculate( (Elapsed / Duration), Ease_);
    // Interpolate
    //Zilch::PropertyDelegateTemplate deleg;
    //Zilch::ExecutableState state;
    //Zilch::ExceptionReport report;
    //call.Set(0, Property);
    //call.Invoke();

    // Set the new value
    Zilch::Call call(Property.Set, Systems::ZilchInterface::GetState());
    call.Set(0, updatedVal);
    call.Invoke(Report);

    if (Elapsed >= Duration) {
      if (DCE_TRACE_ACTIONS_UPDATE)
        DCTrace << "ActionProperty::Update: Finished! \n";
      IsFinished = true;
    }
    else if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    if (timeLeft < dt)
      timeConsumed = dt;
    else
      timeConsumed = timeLeft;

    return timeConsumed;
  }

  /**************************************************************************/
  /*!
  @brief Updates the ActionProperty.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  float ActionProperty<PropertyType>::Update(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    Property = InitialValue + Difference * Easing::Calculate( (Elapsed / Duration), Ease_);

    if (Elapsed >= Duration) {
      if (DCE_TRACE_ACTIONS_UPDATE)
        DCTrace << "ActionProperty::Update: Finished! \n";
      IsFinished = true;
    }
    else if (DCE_TRACE_ACTIONS_UPDATE)
      DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    if (timeLeft < dt)
      timeConsumed = dt;
    else
      timeConsumed = timeLeft;

    return timeConsumed;
  }

}