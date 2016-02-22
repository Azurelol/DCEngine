/*****************************************************************************/
/*!
@file   SerializerJSONCPP.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#define JSON_IS_AMALGAMATION
//#include "..\..\..\Dependencies\include\JSONCPP\json.h"
#include <JSONCPP\json.h>
#include <string>

namespace DCEngine {

  class Entity;
  class Component;
  class System;
  struct ProjectProperties;

  class SerializerJSONCPP {
  public:
    
    template <typename ClassType>
    static bool Serialize(ClassType* object, std::string& output) {
      if (object == NULL)
        return false;
      Json::Value serializeRoot;
      object->Serialize(serializeRoot);

      Json::StyledWriter writer;
      output = writer.write(serializeRoot);
      return true;
    }

    template <typename ClassType>
    static bool Deserialize(ClassType* object, const std::string& input) {
      Json::Value deserializeRoot;
      Json::Reader reader;

      if (!reader.parse(input, deserializeRoot)) {
        DCTrace << "SerializerJSONCPP::Deserialize - Failed due to: \n";
        //for (auto error : reader.parse.errors_) {
        //  DCTrace << error;
        //}
        return false;
      }
        

      object->Deserialize(deserializeRoot);
      return true;
    }


    /*
    
    #define SERIALIZER(TYPE) \
    void Serialize<TYPE>(const TYPE& data, const std::string& varname, Json::Value& root)

    template<typename T>
    void Serialize(const T&data, const std::string& varname, Json::Value& root) {
    root[varname] = data;
    }

    SERIALIZER(ONE_VEC3) {
    Json::Value vec3;
    Serialize(data.x(), "X", vec3);
    Serialize(data.y(), "Y", vec3);
    Serialize(data.z(), "Z", vec3);
    root[varname] = vec3;

    SERIALIZER(ONE_MAT34) {
    Json::Value Mat;
    mat[0][0] = data(0, 0);
    mat[0][1] = data(0, 1);
    mat[0][2] = data(0, 2);
    ..
    root[varname] = mat;


    
    */

  private:


  };


}