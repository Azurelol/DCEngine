#pragma once
#include "SerializerJSONCPP.h"

namespace DCEngine {

  class Serializer {
  public:

    /* Entity Serialization */
    static bool Serialize(Entity* entityObj, std::string& outputFile) {
      return SerializerJSONCPP::Serialize(entityObj, outputFile);
    }
    static bool Deserialize(Entity* entityObj, std::string& inputFile) {
      return SerializerJSONCPP::Deserialize(entityObj, inputFile);
    }

    /* Component Serialization */
    //static bool Serialize(Entity* entityObj, std::string& outputFile) {
    //  return SerializerJSONCPP::Serialize(entityObj, outputFile);
    //}
    //static bool Deserialize(Entity* entityObj, std::string& inputFile) {
    //  return SerializerJSONCPP::Deserialize(entityObj, inputFile);
    //}

  private:

  };
  
  // How to provide an abstract interface for a "static class"???

}