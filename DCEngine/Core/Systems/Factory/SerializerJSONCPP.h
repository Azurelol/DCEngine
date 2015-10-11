#pragma once
#define JSON_IS_AMALGAMATION
//#include "..\..\..\Dependencies\include\JSONCPP\json.h"
#include <JSONCPP\json.h>
#include <string>

namespace DCEngine {

  class Entity;
  class Component;
  class System;
  struct ProjectData;

  class SerializerJSONCPP {
  public:
    
    /* Entity */
    static bool Serialize(Entity* entityObj, std::string& output);
    static bool Deserialize(Entity* entityObj, std::string& input);
    /* Component */
    static bool Serialize(Component* componentObj, std::string& output);
    static bool Deserialize(Component* componentObj, std::string& input);
    /* System */
    static bool Serialize(System* system, std::string& output);
    static bool Deserialize(System* system, std::string& input);
    /* Project Data */
    static bool Serialize(ProjectData* data, std::string& output);
    static bool Deserialize(ProjectData* data, std::string& input);
    //static bool Serialize(Component* componentObj, std::string& output);
    //static bool Deserialize(Component* componentObj, std::string& input);


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