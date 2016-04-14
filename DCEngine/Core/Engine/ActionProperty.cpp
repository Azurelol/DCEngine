/*****************************************************************************/
/*!
@file      ActionProperty.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      3/24/2016
@brief     ActionProperty implementation.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ActionProperty.h"

// Engine
#include "Engine.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief ActionBaseProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  ActionBaseProperty::ActionBaseProperty(ActionSetPtr set, Real duration, Ease ease)
    : Duration(duration), EaseType(ease)
  {
  }

  /**************************************************************************/
  /*!
  @brief Updates the action.
  @param dt The delta time.
  */
  /**************************************************************************/
  float ActionBaseProperty::Update(float dt)
  {
    return Interpolate(dt);
  }  

  /**************************************************************************/
  /*!
  @brief ActionZilchProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param deleg The property delegate.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  ActionZilchProperty::ActionZilchProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg, 
                                            Real duration, Ease ease)
    : ActionBaseProperty(set, duration, ease), Property(deleg)
  {

  }

  
  /**************************************************************************/
  /*!
  @brief ActionZilchBooleanProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param deleg The property delegate.
  @param value The desired end value for the property.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  ActionZilchBooleanProperty::ActionZilchBooleanProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                                                         Boolean value, Real duration, Ease ease)
    : ActionZilchProperty(set, deleg, duration, ease), EndValue(value)
  {
    // Get the initial value of the property
    Zilch::Call call(Property.Get, Systems::ZilchInterface::GetState());
    call.Invoke(Report);
    InitialValue = call.Get<DCEngine::Boolean>(Zilch::Call::Return);
  }

  /**************************************************************************/
  /*!
  @brief Interpolates the property towards the intended value.
  @param dt The delta time.
  */
  /**************************************************************************/
  float ActionZilchBooleanProperty::Interpolate(float dt)
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
      Zilch::Call call(Property.Set, Systems::ZilchInterface::GetState());
      call.Set(0, EndValue);
      call.Invoke(Report);
    }
    else
      timeConsumed = timeLeft;

    return timeConsumed;
  }

                        /* INTEGER */
  /**************************************************************************/
  /*!
  @brief ActionIntegerProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param deleg The property delegate.
  @param value The desired end value for the property.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  ActionIntegerProperty::ActionIntegerProperty(ActionSetPtr set, Integer & prop, Integer value, Real duration, Ease ease)
    : ActionBaseProperty(set, duration, ease), Property(prop), InitialValue(prop), EndValue(value)
  {
    // Calculate the difference
    Difference = EndValue - InitialValue;
  }

  /**************************************************************************/
  /*!
  @brief Interpolates the property towards the intended value.
  @param dt The delta time.
  */
  /**************************************************************************/
  float ActionIntegerProperty::Interpolate(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;
    DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Calculate the interpolated value
    auto currVal = InitialValue + Difference * Easing::Calculate((Elapsed / Duration), EaseType);

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    // If the action is finished
    if (timeLeft <= dt) {
      timeConsumed = dt;
      IsFinished = true;
      Property = EndValue;
    }
    // If there is time left...
    else {
      timeConsumed = timeLeft;
      Property = std::ceil(currVal);
    }

    return timeConsumed;
  }
  /**************************************************************************/
  /*!
  @brief ActionZilchIntegerProperty constructor.
  @param set A pointer to the set this action belongs to.
  @param deleg The property delegate.
  @param value The desired end value for the property.
  @param duration The duration of this action.
  @param ease What ease algorithm to use.
  */
  /**************************************************************************/
  ActionZilchIntegerProperty::ActionZilchIntegerProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg, Integer value, Real duration, Ease ease)
    : ActionZilchProperty(set, deleg, duration, ease), EndValue(value)
  {
    // Get the initial value of the property
    Zilch::Call call(Property.Get, Systems::ZilchInterface::GetState());
    call.Invoke(Report);
    InitialValue = call.Get<DCEngine::Integer>(Zilch::Call::Return);
    // Calculate the difference
    Difference = static_cast<float>(EndValue - InitialValue);
  }

  /**************************************************************************/
  /*!
  @brief Interpolates the property towards the intended value.
  @param dt The delta time.
  */
  /**************************************************************************/
  float ActionZilchIntegerProperty::Interpolate(float dt)
  {
    if (Elapsed == 0)
    {
      // Get the initial value of the property
      Zilch::Call call(Property.Get, Systems::ZilchInterface::GetState());
      call.Invoke(Report);
      InitialValue = call.Get<DCEngine::Integer>(Zilch::Call::Return);
      // Calculate the difference
      Difference = EndValue - InitialValue;
    }
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;
    DCTrace << "ActionProperty::Update: dt = '" << dt << "', timeLeft = '" << timeLeft << "' \n";

    // Calculate the interpolated value
    CurrentValue = InitialValue + Difference * Easing::Calculate((Elapsed / Duration), EaseType);
    // Since it's an integer, cast it
    Zilch::Call call(Property.Set, Systems::ZilchInterface::GetState());

    // Return the time consumed from this action. 
    auto timeConsumed = 0.0f;
    // If the action is finished
    if (timeLeft <= dt) {
      timeConsumed = dt;
      IsFinished = true;
      call.Set(0, EndValue);
      call.Invoke(Report);
    }
    // If there is time left...
    else {
      timeConsumed = timeLeft;
      Integer valAsInteger = static_cast<Integer>(CurrentValue);
      call.Set(0, valAsInteger);
      call.Invoke(Report);
    }


    return timeConsumed;
  }




}