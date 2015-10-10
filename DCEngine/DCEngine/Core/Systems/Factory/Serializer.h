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
    static bool Serialize(Component* componentPtr, std::string& outputFile) {
      return SerializerJSONCPP::Serialize(componentPtr, outputFile);
    }
    static bool Deserialize(Component* componentPtr, std::string& inputFile) {
      return SerializerJSONCPP::Deserialize(componentPtr, inputFile);
    }

    /* System Serialization */
    static bool Serialize(System* systemPtr, std::string& outputFile) {
      return SerializerJSONCPP::Serialize(systemPtr, outputFile);
    }
    static bool Deserialize(System* systemPtr, std::string& inputFile) {
      return SerializerJSONCPP::Deserialize(systemPtr, inputFile);
    }

  private:

  };
  
  // How to provide an abstract interface for a "static class"???

}