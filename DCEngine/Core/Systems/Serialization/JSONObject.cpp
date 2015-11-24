/*****************************************************************************/
/*!
@file   JSONObject.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The JSONObject is a wrapper for serialized meta data in JSON.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "JSONObject.h"

namespace DCEngine {

  JSONObject::JSONObject(Zilch::JsonValue * value) : Value(value), IsValue(true)
  {
  }

  JSONObject::JSONObject(Zilch::JsonMember * member) : Member(member), IsValue(false)
  {
  }

  Zilch::JsonValue * JSONObject::GetValue()
  {
    return this->Value;
  }

  Zilch::JsonMember * JSONObject::GetMember()
  {
    return this->Member;
  }

}