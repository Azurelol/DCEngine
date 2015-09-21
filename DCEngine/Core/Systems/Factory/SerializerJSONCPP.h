#pragma once
#define JSON_IS_AMALGAMATION
//#include "..\..\..\Dependencies\include\JSONCPP\json.h"
#include <JSONCPP\json.h>
#include <string>

namespace DCEngine {

  class Entity;

  class SerializerJSONCPP {
  public:
    static bool Serialize(Entity* entityObj, std::string& outputFile);
    static bool Deserialize(Entity* entityObj, std::string& inputFile);

  private:


  };


}