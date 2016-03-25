#pragma once
#include "Action.h"

// State
#include "../Systems/Reflection/ZilchInterface.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @class ActionBaseProperty Base ActionProperty class from which the other
         property classes derive from.
  */
  /**************************************************************************/
  class ActionBaseProperty : public Action {
    ActionBaseProperty(ActionSetPtr set, Real duration, Ease ease);
    float Update(float dt);
    virtual float Interpolate(float dt) = 0;
  private:
    Real Duration;
    Ease EaseType;
  };

  class ActionBoolProperty : public ActionBaseProperty {
  public:
    float Interpolate(float dt);

  private:
    Boolean InitialValue;
    Boolean EndValue;
  };

  /**************************************************************************/
  /*!
  @class ActionProperty A type of action that modifies the value of
  a given property over a specified amount of time, using a specified
  interpolation formula (Ease).
  */
  /**************************************************************************/
  template <typename PropertyType>
  class ActionProperty : public Action {
  public:
    ActionProperty(ActionSetPtr set, PropertyType& prop, PropertyType value, Real duration, Ease ease);
    float Update(float dt);

  private:
    PropertyType& Property;
    PropertyType Difference;
    PropertyType InitialValue;
    PropertyType EndValue;
    Real Duration;
    Ease Ease_;
  };

  // Variant for Zilch
  template <typename PropertyType>
  class ActionZilchProperty : public Action {
  public:
    ActionZilchProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate prop, PropertyType value, Real duration, Ease ease);
    float Update(float dt);
  private:
    Zilch::PropertyDelegateTemplate Property;
    Zilch::ExceptionReport Report;
    PropertyType Difference;
    PropertyType InitialValue;
    PropertyType EndValue;
    Real Duration;
    Ease Ease_;
  };

}

#include "ActionProperty.hpp"