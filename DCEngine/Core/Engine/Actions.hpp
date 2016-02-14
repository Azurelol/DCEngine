/*****************************************************************************/
/*!
@file      Action.hpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      2/12/2016
@brief     Action template file.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Action.h"

namespace DCEngine {

  /*==============*
  *   Property    *
  *==============*/
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
  @brief Updates the ActionProperty.
  @param dt The time slice given.
  */
  /**************************************************************************/
  template<typename PropertyType>
  float ActionProperty<PropertyType>::Update(float dt)
  {
    Elapsed += dt;
    auto timeLeft = Duration - Elapsed;

    // Interpolate
    Property = InitialValue + Difference * CalculateEase(Elapsed / Duration); 

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
  @brief Calculates the Ease value.
  @param t A number.
  @return A real.
  */
  /**************************************************************************/
  template<typename PropertyType>
  inline Real ActionProperty<PropertyType>::CalculateEase(Real t)
  {
    Real easeVal;
    switch (Ease_) {
    case Ease::Linear:
      easeVal = Easing::Linear(t);
      break;
    case Ease::QuadIn:
      easeVal = Easing::QuadIn(t);
      break;
    case Ease::QuadOut:
      easeVal = Easing::QuadOut(t);
      break;
    }
    return easeVal;
  }

  /*==============*
  *   Actions     *
  *==============*/
  /**************************************************************************/
  /*!
  @brief Creates an ActionCall and adds it to the specified set.
  @param set A reference to the set.
  @param func The function to which to call.
  @param object The object on which to invoke the function on.
  @param ... Variadic arguments for the member function.
  */
  /**************************************************************************/
  template<typename Class, typename ...Args>
  void Actions::Call(ActionSetPtr set, void(Class::* func)(Args...), Class * object, Args ...)
  {
    //auto a = std::bind(std::mem_fun(func), object, Args...);
    auto deleg = new MemberFunctionDelegate<Class>(object, func);
    // Construct an ActionCall object
    ActionPtr call(new ActionCall(set, deleg));
    // Add it to the set
    set->Add(call);
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionProperty and adds it to the specified set.
  @param PropertyType The POD or class type of the property;
  @param set A reference to the set this property is part of.
  @param property A reference to the property to be modified.
  @param value The new value to interpolate over the given duration.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/
  template<typename PropertyType>
  void Actions::Property(ActionSetPtr set, PropertyType & propertyRef, PropertyType val, Real duration, Ease ease)
  {
    // Construct an ActionProperty object
    ActionPtr prop(new ActionProperty<PropertyType>(set, propertyRef, val, duration, ease));
    // Add it to the set
    set->Add(prop);
  }

}