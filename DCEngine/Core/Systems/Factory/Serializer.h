/******************************************************************************/
/*!
@file   Serializer.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   7/30/2015
@brief  The interface for the serializer functions for the engine.
@todo   May need to use different implementation for serialization so we don't
        have to write our own serialization functions for every individual
        component.

*/
/******************************************************************************/
#pragma once
#include "SerializerJSONCPP.h"

namespace DCEngine {

  class Serializer {
  public:

    template <typename ClassType>
    static bool Serialize(ClassType* object, std::string& output) {
      return SerializerJSONCPP::Serialize(object, output);
    }

    template <typename ClassType>
    static bool Deserialize(ClassType* object, const std::string& input) {
      return SerializerJSONCPP::Deserialize(object, input);
    }

    ///* Entity Serialization */
    //static bool Serialize(Entity* entityObj, std::string& outputFile) {
    //  return SerializerJSONCPP::Serialize(entityObj, outputFile);
    //}
    //static bool Deserialize(Entity* entityObj, std::string& inputFile) {
    //  return SerializerJSONCPP::Deserialize(entityObj, inputFile);
    //}

    ///* Component Serialization */
    //static bool Serialize(Component* componentPtr, std::string& outputFile) {
    //  return SerializerJSONCPP::Serialize(componentPtr, outputFile);
    //}
    //static bool Deserialize(Component* componentPtr, std::string& inputFile) {
    //  return SerializerJSONCPP::Deserialize(componentPtr, inputFile);
    //}

    ///* System Serialization */
    //static bool Serialize(System* systemPtr, std::string& outputFile) {
    //  return SerializerJSONCPP::Serialize(systemPtr, outputFile);
    //}
    //static bool Deserialize(System* systemPtr, std::string& inputFile) {
    //  return SerializerJSONCPP::Deserialize(systemPtr, inputFile);
    //}

    /* Data Serialiization */



  private:

  };
  
  // How to provide an abstract interface for a "static class"???

}