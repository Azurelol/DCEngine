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

  /*===================*
  *     Properties     *
  *===================*/
  // This macro creates the definitions of getter and setter method for a Property.
#define DCE_DEFINE_PROPERTY(propertyType, propertyName)                         \
  const propertyType& get##propertyName() const {                                      \
    return propertyName;                                                          \
  }                                                                             \
  void set##propertyName(propertyType const& value) {                             \
    propertyName = value;                                                         \
  }                                                            


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


    
