#pragma once
#include <memory>
#include <string>
#include <map>

#include "MetaData.h"

class MetaManager {
public:
  using MetaMap = std::map <std::string, const MetaData>;

  // Inserts a MetaData int othe map of ojects
  static void RegisterMeta(const MetaData* instance);
  // Retrieve a MetaData instance by string name from the map of MetaData objects
  static const MetaData* Get(std::string name);
  // Safe and easy singleton for map of MetaData objects
  static MetaMap GetMap() {
    // Defines the static map here so there's no need for explicit definitions
    static MetaMap map;
    return map;
  }
};

/* Macros for registering a type within the MetaData system */

/* META_TYPE: Retrieves the proper MetaaData instance of an object by type. */
#define META_TYPE( TYPE ) (MetaCreator::Get( ))
/* META: Retrieves the proper MetaData instance of an object by an object's type*/
#deifne META(OBJECT) (MetaCreator)