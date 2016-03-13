/*****************************************************************************/
/*!
@file   JSONObject.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The JSONObject is a wrapper for serialized meta data in JSON.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
//#include <ZILCH\Zilch.hpp>

namespace DCEngine {
  
    class JSONObject {
    public:

      JSONObject(Zilch::JsonValue*);
      JSONObject(Zilch::JsonMember*);
      template <typename JSONType>
      JSONType Get();
      Zilch::JsonValue* GetValue();
      Zilch::JsonMember* GetMember();
      template <typename JSONType>
      JSONType* operator->() const;

    private:
      bool IsValue;
      // A 'value' in JSON can be a string, number, object, array or null 
      Zilch::JsonValue* Value;
      // A 'member' is basically a key-value pair, where the key is always a
      // string and the value is generic
      Zilch::JsonMember* Member;

    };

  
    template<typename JSONType>
    inline JSONType JSONObject::Get()
    {
      if (IsValue)
        return Value;
      else
        return Member;
    }

    template<typename JSONType>
    JSONType* JSONObject::operator->() const
    {
      if (IsValue)
        return Value;
      else
        return Member;
    }

}