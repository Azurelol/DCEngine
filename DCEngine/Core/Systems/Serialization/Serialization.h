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

*/
/******************************************************************************/
#pragma once
#include "SerializerJSONCPP.h"
//#include "../Reflection/ReflectionMeta.h"
//#include "../Reflection/MetaData.h"
#include "../Reflection/RefVariant.h"

namespace DCEngine {

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

    static void Padding(std::ostream& os);
    static void SerializeText(std::ostream& os, RefVariant var);    
    template <typename T> static void SerializeTextPrimitive(std::ostream& os, RefVariant prim);

  };

  template<typename T>
  inline void Serialization::SerializeTextPrimitive(std::ostream & os, RefVariant prim)
  {
    Padding(os);
    os << prim.GetValue<RemoveQualifier<T>::type>() << std::endl;
  }

}