/*****************************************************************************/
/*!
@file   ReflectionMeta.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/16/2015
@brief  The reflection system provides a way to store data about class
        and data types at runtime that the compiler usually discards.
@note   The credit to this implementation of a Reflection system goes the 
        following DigiPen alumni:
        - Randy Gaul
        - Sean Reilly
        - Treb Connell
      
        Randy Gaul's C++ Reflection: Type-MetaData tutorial series:
        http://www.randygaul.net/2012/10/01/c-reflection-type-metadata-introduction/
*/
/******************************************************************************/
#pragma once
#include "MetaData.h"
#include "Variant.h"
#include "RefVariant.h"
#include "StringWrapper.h"

#include "../../Engine/Types.h"
#include "../../Debug/Debug.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief The MetaManager is a non-templated class that contains all of the 
         created MetaData instances, and can perform search operations on them
         to find specific types. We use a map of strings to instances so
         we can search by string identifier.
  */
  /**************************************************************************/
  class MetaManager {
  public:
    using MetaMap = std::map<std::string, const MetaData*>;

    //!< Inserts a MetaData into a map of objects.
    static void RegisterMeta(const MetaData* instance);
    //!< Retrieves a MetaData instance by string name from the map of MetaData
    //  objects.
    static const MetaData* Get(std::string name) {
      return GetMap().at(name);
    }

    //!< Safe and easy singleton for a map of MetaData objects
    static MetaMap& GetMap() {
      static MetaMap map;
      return map;
    }
  };

  /**************************************************************************/
  /*!
  @brief Prints the member variables of a given object.
  */
  /**************************************************************************/
  template <typename TYPE>
  void PrintType(const char *type)
  {
    DCTrace << "\nMetaData for " << type << ": \n";
    DCTrace << "Name: " << META_TYPE(TYPE)->Name() << "\n";
    DCTrace << "Size: " << META_TYPE(TYPE)->Size() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief Prints the member variables of a given object.
  */
  /**************************************************************************/
  template <typename TYPE>
  void PrintMembers(const char *type)
  {
    DCTrace << "\nMetaData for " << type << ": \n";
    DCTrace << "Name: " << META_TYPE(TYPE)->Name() << "\n";
    DCTrace << "Size: " << META_TYPE(TYPE)->Size() << "\n";
    DCTrace << "Members: \n";
    DCTrace << "{\n";
    auto cont = META_TYPE(TYPE)->MemberContainer;
    for (auto i = META_TYPE(TYPE)->MemberContainer.begin(); i != META_TYPE(TYPE)->MemberContainer.end(); ++i)
    {
      DCTrace << "  " << (*i)->Name() << "\n";
      DCTrace << "  " << (*i)->Offset() << "\n";
    }
    DCTrace << "}\n";
  }

}