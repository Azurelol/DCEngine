#pragma once
#include "MetaData.h"
#include "../../Engine/Types.h"

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




}