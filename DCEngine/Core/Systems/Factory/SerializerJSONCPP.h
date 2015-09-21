#pragma once
#define JSON_IS_AMALGAMATION
//#include "..\..\..\Dependencies\include\JSONCPP\json.h"
#include <JSONCPP\json.h>
#include <string>

namespace DCEngine {

  class Entity;
  class Component;

  class SerializerJSONCPP {
  public:
    static bool Serialize(Entity* entityObj, std::string& output);
    static bool Deserialize(Entity* entityObj, std::string& input);

    static bool Serialize(Component* componentObj, std::string& output);
    static bool Deserialize(Component* componentObj, std::string& input);

  private:


  };


}