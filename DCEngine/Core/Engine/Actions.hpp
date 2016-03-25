/*****************************************************************************/
/*!
@file      Actions.hpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      2/12/2016
@brief     Actions template file.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Actions.h"

namespace DCEngine {

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
  void Actions::PropertyT(ActionSetPtr set, PropertyType & propertyRef, PropertyType val, Real duration, Ease ease)
  {
    // Construct an ActionProperty object
    ActionPtr prop(new ActionProperty<PropertyType>(set, propertyRef, val, duration, ease));
    // Add it to the set
    set->Add(prop);
  }

  // Copy due to memory
  template<typename PropertyType>
  void Actions::PropertyZilch(ActionSetPtr set, Zilch::PropertyDelegateTemplate propertyDeleg, PropertyType val, Real duration, Ease ease)
  {    
    // Construct an ActionProperty object
    ActionPtr prop(new ActionZilchProperty<PropertyType>(set, propertyDeleg, val, duration, ease));
    // Add it to the set
    set->Add(prop);
  }

}