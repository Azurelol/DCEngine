/*****************************************************************************/
/*!
@file   Macros.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/14/2015
@brief  A collection of macros to alleviate rote code writing!
@note   MACROS.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "MacrosEvents.h"
#include "MacrosBinding.h""

namespace DCEngine {

  #define DEFER(X) X

  /*===================*
  *     Components     *
  *===================*/
  // This macro declares a component..
  using DependenciesContainer = std::vector<std::string>;

  // Declares dependencies for a given component
  #define DCE_COMPONENT_DECLARE_DEPENDENCIES \
  static DependenciesContainer mDependencies; \
  virtual DependenciesContainer& Dependencies() const noexcept override { return mDependencies; }

  // Defines the dependencies for a given component
  #define DCE_COMPONENT_DEFINE_DEPENDENCIES(componentName, ...) \
  DependenciesContainer componentName::mDependencies = { __VA_ARGS__ } 


  /*===================*
  *     Properties     *
  *===================*/
  // This macro creates the definitions of getter and setter method for a Property.
  #define DCE_DEFINE_PROPERTY(propertyType, propertyName)                         \
  const propertyType& get##propertyName() const {                                 \
    return propertyName;                                                          \
  }                                                                               \
  void set##propertyName(propertyType const& value) {                             \
    propertyName = value;                                                         \
  }

  #define DCE_DEFINE_GETTER(propertyType, propertyName)                           \
  const propertyType& get##propertyName() const {                                 \
    return propertyName;                                                          \
  }

  #define DCE_DEFINE_SETTER(propertyType, propertyName)                           \
  void set##propertyName(propertyType const& value) {                             \
    propertyName = value;                                                         \
  }
  
  /*===================*
  *     Enumerations   *
  *===================*/
  #define DCE_DECLARE_ENUM(enumType, ...)   \
  enum class enumType {                             \
  __VA_ARGS__                                     \
  }
  // This macro defines a property to Zilch
  #define DCE_BINDING_DEFINE_ENUM(enumType, enumField)          \
  auto property##enumField = ZilchBindEnumValue(builder, type, ##enumType::##enumField, "" #enumField) 

  }



