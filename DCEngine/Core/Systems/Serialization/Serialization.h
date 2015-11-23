/******************************************************************************/
/*!
@file   Serialization.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   7/30/2015
@brief  The interface for the serializer functions for the engine.
@todo   May need to use different implementation for serialization so we don't
        have to write our own serialization functions for every individual
        component.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "SerializerJSONCPP.h"
#include "JSONObject.h"

namespace DCEngine {

  //class SerializedData {
  //public:
  //  SerializedData(std::string& data) {

  //  }
  //  ~SerializedData() {

  //  }

  //private:
  //  JSONObject JSONObject;
  //};

  using SerializedData = JSONObject;
  using SerializedMember = Zilch::JsonMember;

  class Serialization {
  public:

    template <typename ClassType>
    static bool Serialize(ClassType* object, std::string& output) {
      return SerializerJSONCPP::Serialize(object, output);
    }
    template <typename ClassType>
    static bool Deserialize(ClassType* object, const std::string& input) {
      return SerializerJSONCPP::Deserialize(object, input);
    }





  };


}