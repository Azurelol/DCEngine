#pragma once
#include "Action.h"

// State
#include "../Systems/Reflection/ZilchInterface.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @class ActionBaseProperty Base property class from which the other
         property classes derive from.
  */
  /**************************************************************************/
  class ActionBaseProperty : public Action {
  public:
    ActionBaseProperty(ActionSetPtr set, Real duration, Ease ease);
    float Update(float dt);
    virtual float Interpolate(float dt) = 0;
  protected:
    Real Duration;
    Ease EaseType;
  };

                                    /* ZILCH */
  /**************************************************************************/
  /*!
  @class ActionZilchProperty Property class from which all properties
         created through Zilch derive from.
  */
  /**************************************************************************/
  class ActionZilchProperty : public ActionBaseProperty {
  public:
    ActionZilchProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg, Real duration, Ease ease);
    virtual float Interpolate(float dt) = 0;
  protected:
    Zilch::PropertyDelegateTemplate Property;
    Zilch::ExceptionReport Report;
  };

  /**************************************************************************/
  /*!
  @class ActionZilchFloatProperty Property class for floating point values.
  */
  /**************************************************************************/
  template <typename PropertyType>
  class ActionZilchFloatProperty : public ActionZilchProperty {
  public:
    ActionZilchFloatProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                             PropertyType value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    PropertyType Difference;
    PropertyType InitialValue;
    PropertyType EndValue;
  };

  /**************************************************************************/
  /*!
  @class ActionZilchStringProperty Property class for String values.
  */
  /**************************************************************************/
  class ActionZilchStringProperty : public ActionZilchProperty {
  public:
    ActionZilchStringProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                              String value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    String InitialValue;
    String EndValue;
  };

  /**************************************************************************/
  /*!
  @class ActionZilchStringProperty Property class for String values.
  */
  /**************************************************************************/
  class ActionZilchBooleanProperty : public ActionZilchProperty {
  public:
    ActionZilchBooleanProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                               Boolean value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    Boolean InitialValue;
    Boolean EndValue;
  };

  /**************************************************************************/
  /*!
  @class ActionZilchStringProperty Property class for String values.
  */
  /**************************************************************************/
  class ActionZilchIntegerProperty : public ActionZilchProperty {
  public:
    ActionZilchIntegerProperty(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                               Integer value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    Integer InitialValue;
    Integer EndValue; 
    Real Difference;
    Real CurrentValue;
  };
  
  /**************************************************************************/
  /*!
  @class ActionZilchIntegerProperty Property class for Integer values.
  */
  /**************************************************************************/
  template <typename IntegerType>
  class ActionZilchIntegerPropertyT : public ActionZilchProperty {
  public:
    ActionZilchIntegerPropertyT(ActionSetPtr set, Zilch::PropertyDelegateTemplate deleg,
                               IntegerType value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    IntegerType InitialValue;
    IntegerType CurrentValue;
    IntegerType EndValue;
  };

  /**************************************************************************/
  /*!
  @class ActionZilchBooleanProperty Property class for Boolean values.
  */
  /**************************************************************************/
  template <typename BooleanType>
  class ActionZilchBooleanPropertyT : public ActionZilchProperty {
  public:
    ActionZilchBooleanPropertyT(ActionSetPtr set, Zilch::PropertyDelegateTemplate prop,
                               BooleanType value, Real duration, Ease ease);
    float Interpolate(float dt);
  private:
    BooleanType InitialValue;
    BooleanType EndValue;
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
  class ActionZilchFloatPropertyOld : public Action {
  public:
    ActionZilchFloatPropertyOld(ActionSetPtr set, Zilch::PropertyDelegateTemplate prop, 
                                PropertyType value, Real duration, Ease ease);
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