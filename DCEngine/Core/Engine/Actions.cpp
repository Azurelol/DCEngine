/*****************************************************************************/
/*!
@file      Actions.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/7/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Actions.h"

#include "Engine.h"

namespace DCEngine {

  unsigned Action::Created = 0;
  unsigned Action::Destroyed = 0;

  /*!************************************************************************\
  @brief  Action Zilch Definition
  \**************************************************************************/
  ZilchDefineType(Action, "Action", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    //ZilchBindConstructor(builder, type, Action, "type", std::string);
    //ZilchBindDestructor(builder, type, Action);
  }

  ZilchDefineType(ActionSet, "ActionSet", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    //ZilchBindConstructor(builder, type, ActionSet, "type", std::string);
    //ZilchBindDestructor(builder, type, ActionSet);
  }

  ZilchDefineType(ActionSequence, "ActionSequence", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    ZilchBindConstructor(builder, type, ActionSequence, ZilchNoNames);
    ZilchBindDestructor(builder, type, ActionSequence);
  }

  ZilchDefineType(ActionGroup, "ActionGroup", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    ZilchBindConstructor(builder, type, ActionGroup, ZilchNoNames);
    ZilchBindDestructor(builder, type, ActionGroup);
  }

  ZilchDefineType(ActionsOwner, "ActionsOwner", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;    
    ZilchBindConstructor(builder, type, ActionsOwner, "owner", Entity&);
    ZilchBindDestructor(builder, type, ActionsOwner);
  }

  /**************************************************************************/
  /*!
  @brief Action constructor
  */
  /**************************************************************************/
  Action::Action(std::string type) : Type(type), Elapsed(0.0f), Duration(0.0f), ID(Created++)
  {
  }

  /**************************************************************************/
  /*!
  @brief Action destructor
  */
  /**************************************************************************/
  Action::~Action()
  {
    Destroyed++;
  }

  /*==============*
  *     Actions   *
  *==============*/
  /**************************************************************************/
  /*!
  @brief Creates an action sequence.
  @param owner A reference to the owner of this action sequence.
  */
  /**************************************************************************/
  ActionSetPtr Actions::Sequence(ActionSet & owner)
  {
    // Construct an action group
    ActionSetPtr sequence(new ActionSequence());
    // Add this action group to the set
    owner.Add(sequence);
    // Return a reference to this action group
    return sequence;
  }

  /**************************************************************************/
  /*!
  @brief Creates an action sequence.
  @param owner A reference to the owner of this action sequence.
  */
  /**************************************************************************/
  ActionSetPtr Actions::Group(ActionSet & owner)
  {    
    // Construct an action group
    ActionSetPtr group(new ActionGroup());
    // Add this action group to the set
    owner.Add(group);
    // Return a reference to this action group
    return group;
  }



  void EaseMeUp(Ease ease) {

  }



  /*!************************************************************************\
  @brief  Actions Zilch Definition
  \**************************************************************************/
  ZilchDefineType(Actions, "Actions", DCEngineCore, builder, type) {
    // Methods
    ZilchBindMethod(builder, type, &Actions::Sequence, ZilchNoOverload, "Sequence", "owner");
    ZilchBindMethod(builder, type, &Actions::Group, ZilchNoOverload, "Group", "owner");
    ZilchBindMethod(builder, type, &Actions::Delay, ZilchNoOverload, "Delay", "setRef, duration");

    // Call
    Zilch::ParameterArray callParams;
    callParams.push_back(ZilchTypeId(ActionSetPtr));  
    callParams.push_back(Zilch::Core::GetInstance().AnyDelegateType);
    builder.AddBoundFunction(type, "Call", Actions::Call, callParams, ZilchTypeId(void), 
                             Zilch::FunctionOptions::Static);
    // Properties
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Real, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Integer, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Boolean, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Vec2, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Vec3, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");
    ZilchBindMethod(builder, type, &Actions::Property, (void(*)(ActionSetPtr, Zilch::Handle, Vec4, Real, Ease)),
                                                        "Property", "setRef, prty, val, duration, ease");

  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionDelay
  @param set A reference to the ActionSet that this action belongs to.
  @param duration How long should the delay run for.
  */
  /**************************************************************************/
  void Actions::Delay(ActionSetPtr set, Real duration)
  {
    // Construct the action and add it to the action set
    ActionPtr call(new ActionDelay(set, duration));
    // Add it to the set
    set->Add(call);
  }

  /*============================
        ACTION PROPERTIES  
  ===========================*/  
  void Actions::Property(ActionSetPtr set, Real & prty, Real val, Real duration, Ease ease)
  {
    PropertyT(set, prty, val, duration, ease);
  }

  void Actions::Property(ActionSetPtr set, Boolean & prty, Boolean val, Real duration, Ease ease)
  {    
    PropertyT(set, prty, val, duration, ease);
  }

  void Actions::Property(ActionSetPtr set, Integer & prty, Integer val, Real duration, Ease ease)
  {
    PropertyT(set, prty, val, duration, ease);
  }

  void Actions::Property(ActionSetPtr set, Vec2 & prty, Vec2 val, Real duration, Ease ease)
  {
    PropertyT(set, prty, val, duration, ease);
  }

  void Actions::Property(ActionSetPtr set, Vec3 & prty, Vec3 val, Real duration, Ease ease)
  {
    PropertyT(set, prty, val, duration, ease);
  }

  void Actions::Property(ActionSetPtr set, Vec4 & prty, Vec4 val, Real duration, Ease ease)
  {
    PropertyT(set, prty, val, duration, ease);
  }

  /*===============================
        ACTION PROPERTIES (ZILCH)
  ===============================*/
  /**************************************************************************/
  /*!
  @brief Creates an ActionCall.
  @param set A reference to the ActionSet that this action belongs to.
  @param duration How long should the delay run for.
  */
  /**************************************************************************/
  void Actions::Call(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    auto set = reinterpret_cast<ActionSetPtr>(call.Get<ActionSetPtr>(0));
    auto& deleg = call.GetDelegate(1);
    // Create the action delegate that will be used
    auto zilcMemFnDeleg = new ZilchMemberFunctionDelegate();
    zilcMemFnDeleg->State = call.GetState();
    zilcMemFnDeleg->Delegate = deleg;
    // Construct the ActionCall object
    ActionPtr actionCall(new ActionCall(set, zilcMemFnDeleg));
    set->Add(actionCall);    
  }

  /**************************************************************************/
  /*!
  @brief Creates an ActionProperty (Real)
  @param setRef A reference to the ActionSet that this action belongs to.
  @param handle A handle to the property delegate.
  @param val The new value to interpolate over the given duration.
  @param duration How long this property runs for.
  @param ease What ease this property uses to calculate the interpolation.
  */
  /**************************************************************************/
  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Real val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchFloatProperty<Real>(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);    
  }

  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Boolean val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchBooleanProperty(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);
  }

  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Integer val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchIntegerProperty(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);
  }
  
  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec2 val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchFloatProperty<Vec2>(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);
  }

  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec3 val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchFloatProperty<Vec3>(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);
  }

  void Actions::Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec4 val, Real duration, Ease ease)
  {
    auto propertyDelegate = (Zilch::PropertyDelegateTemplate*)propertyHandle.Dereference();
    ActionPtr prop(new ActionZilchFloatProperty<Vec4>(setRef, *propertyDelegate, val, duration, ease));
    setRef->Add(prop);
  }

}