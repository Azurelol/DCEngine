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

  
  #define DCE_BINDING_DECLARE_COMPONENT(COMP)   \
  class COMP;                                   \
  class COMP : public Component  {              \
    public:                                     \
  ZilchDeclareDerivedType(COMP, Component)
  
  
  // This macro declares a derived type to Zilch
  #define DCE_BINDING_DECLARE_DERIVED_TYPE(derivedClass, baseClass)  \
    ZilchDeclareDerivedType(derivedClass, baseClass)  

  // This macro defines a property to Zilch
  #define DCE_BINDING_DEFINE_PROPERTY(className, propertyName)          \
  ZilchBindProperty(builder, type, &className::get##propertyName, &className::set##propertyName, "" #propertyName) 
  // ZilchBindProperty(builder, type, &SoundEmitter::getTranslation, &SoundEmitter::setTranslation, "Translation");

  //// This macro creates the definitions of getter and setter method for a Property.
  //#define DCE_DEFINE_PROPERTY(propertyClass, propertyType, propertyName)        \
  //propertyType propertyClass::get#propertyName() const                          \
  //{                                                                             \
  //  return this->propertyName;                                                  \
  //}                                                                             \
  //void propertyClass::set#propertyName(propertyType value)                      \
  //{                                                                             \
  //  this->propertyName = value;                                                 \
  //}                                                                             

}


    
