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

namespace DCEngine {

  #define DEFER(X) X

  /*===================*
  *     Components     *
  *===================*/
  // This macro declares a component..
  using DependenciesContainer = std::vector<std::string>;

  // Declares dependencies for a component
  #define DCE_COMPONENT_DECLARE_DEPENDENCIES \
  static DependenciesContainer mDependencies; \
  virtual DependenciesContainer& Dependencies() const noexcept override { return mDependencies; }

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

  /*===================*
  *     Binding       *
  *===================*/
  // This macro declares a component 
  #define DCE_DECLARE_COMPONENT(COMP)   \
  class COMP;                                   \
  class COMP : public Component  {              \
    public:                                     \
  ZilchDeclareDerivedType(COMP, Component)

  // This macro declares a derived type to Zilch
  #define DCE_BINDING_DECLARE_DERIVED_TYPE(derivedClass, baseClass)  \
    ZilchDeclareDerivedType(derivedClass, baseClass)  

  // This macro defines a component to zilch
  #define DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(componentName)          \
  ZilchBindConstructor(builder, type, componentName, "owner", Entity&);  \
  ZilchBindDestructor(builder, type, componentName)

  // This macro defines a property to Zilch
  #define DCE_BINDING_DEFINE_PROPERTY(className, propertyName)          \
  auto property##propertyName = ZilchBindProperty(builder, type, &className::get##propertyName, &className::set##propertyName, "" #propertyName) 


  // This macro defines a Zilch attribute
  #define DCE_BINDING_DEFINE_ATTRIBUTE(name) \
  Zilch::Attribute attribute##name; \
  attribute##name.Name = "" #name      

  // This macro sets a Resource-specific attribute onto a property
  #define DCE_BINDING_PROPERTY_SET_ATTRIBUTE(property, attribute) \
  property->Attributes.push_back(attribute);                       \
  property->Attributes.push_back(attributeResource)

  /////////////
  // Resources
  /////////////
  // This macro defines a Resource-specific attribute.
  #define DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(resource) \
  Zilch::Attribute attributeResource;                     \
  attributeResource.Name = "Resource";                    \
  Zilch::Attribute attribute##resource;                   \
  attribute##resource.Name = "" #resource

  // This macro sets a Resource-specific attribute onto a property
  #define DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(property, attribute) \
  property->Attributes.push_back(attributeResource);                     \
  property->Attributes.push_back(attribute)

  #define DCE_BINDING_PROPERTY_DEFINE_UNSIGNED                           \
    Zilch::Attribute attributeUnsigned;                                  \
    attributeUnsigned.Name = "Unsigned"                                    

  #define DCE_BINDING_PROPERTY_SET_UNSIGNED(property)                    \
    property->Attributes.push_back(attributeUnsigned)

  #define DCE_BINDING_PROPERTY_DEFINE_RANGE                             \
    Zilch::Attribute attributeRange;                                    \
    attributeRange.Name = "Range";                                      \
    Zilch::AttributeParameter minRange, maxRange;                       \
    minRange.NumberValue = 0;                                           \
    maxRange.NumberValue = 0;                                           \
    attributeRange.Parameters.push_back(minRange);                      \
    attributeRange.Parameters.push_back(maxRange)

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

//#define DCE_BINDING_DEFINE_ENUM(enumType, library, ...)    \
//ZilchDefineExternalType(DCEngine::ProjectionMode, "ProjectionMode", DCEngine::DCEngineCore, builder, type) {
//  ZilchBindEnum(builder, type, SpecialType::Enumeration);
//
//  ZilchBindEnumValue(builder, type, DCEngine::ProjectionMode::Orthographic, "Orthographic");
//  ZilchBindEnumValue(builder, type, DCEngine::ProjectionMode::Perspective, "Perspective");
//  }

  }



