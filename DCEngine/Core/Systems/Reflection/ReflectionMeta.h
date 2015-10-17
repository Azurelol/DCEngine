#pragma once
#include "MetaData.h"
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
    static const MetaData* Get(std::string name);

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
    trace << "\nMetaData for " << type << ": \n";
    trace << "Name: " << META_TYPE(TYPE)->Name() << "\n";
    trace << "Size: " << META_TYPE(TYPE)->Size() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief Prints the member variables of a given object.
  */
  /**************************************************************************/
  template <typename TYPE>
  void PrintMembers(const char *type)
  {
    trace << "\nMetaData for " << type << ": \n";
    trace << "Name: " << META_TYPE(TYPE)->Name() << "\n";
    trace << "Size: " << META_TYPE(TYPE)->Size() << "\n";
    trace << "Members: \n";
    trace << "{\n";
    auto cont = META_TYPE(TYPE)->MemberContainer;
    for (auto i = META_TYPE(TYPE)->MemberContainer.begin(); i != META_TYPE(TYPE)->MemberContainer.end(); ++i)
    {
      trace << "  " << (*i)->Name() << "\n";
      trace << "  " << (*i)->Offset() << "\n";
    }
    trace << "}\n";
  }

}