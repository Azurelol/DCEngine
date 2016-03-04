/*****************************************************************************/
/*!
@file   MacrosBinding.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/3/2016
@brief  A collection of macros to alleviate rote code writing!
@note   MACROS.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

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
  property->Attributes.push_back(attribute)              

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

// This macro defines the unsigned attribute for the scope of the binding
#define DCE_BINDING_PROPERTY_DEFINE_UNSIGNED                           \
    Zilch::Attribute attributeUnsigned;                                  \
    attributeUnsigned.Name = "Unsigned"                                    

// This macro sets the unsigned attribute on the given property
#define DCE_BINDING_PROPERTY_SET_UNSIGNED(property)                    \
    property->Attributes.push_back(attributeUnsigned)

// This macro defines a range attribute of a given min and max with a given name
#define DCE_BINDING_PROPERTY_DEFINE_RANGE(property, min, max)         \
    Zilch::Attribute attributeRange##property;                          \
    attributeRange##property.Name = "Range";                            \
    Zilch::AttributeParameter minRange##property, maxRange##property;   \
    minRange##property.NumberValue = min;                               \
    maxRange##property.NumberValue = max;                               \
    attributeRange##property.Parameters.push_back(minRange##property);  \
    attributeRange##property.Parameters.push_back(maxRange##property)

// This macro defines the color range attribute 
#define DCE_BINDING_PROPERTY_DEFINE_COLOR_RANGE                       \
  DCE_BINDING_PROPERTY_DEFINE_RANGE(Color, 0, 1)  