/*****************************************************************************/
/*!
@file   Archetype.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/14/2015
@brief  An archetype is a resource that contains the serialized data definition
        of an object. The archetype stores a binary cache of the serialization
        data and the source file for debugging and for archetype updating.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
//#include "..\Objects\Entities\GameObject.h"

namespace DCEngine {

  class Archetype;
  using ArchetypePtr = std::shared_ptr<Archetype>;

  class Archetype : public Resource {
  public:
    Archetype(std::string archetypeFile);
    Archetype(std::string archetypeFile, std::string serializedData);
    ~Archetype();
    static std::string Extension() { return ".Archetype"; }
    static ArchetypePtr Find(std::string);
    void Save();
    bool Load();
    const std::string& Get();
    DCE_DEFINE_PROPERTY(EntityType, Type);

  private:
    EntityType Type;

  };

  using ArchetypeHandle = std::string;
  using ArchetypeContainer = std::vector<ArchetypePtr>;

}

