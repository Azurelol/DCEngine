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
#include "..\Objects\Entities\GameObject.h"

namespace DCEngine {

  class Archetype : public Resource {
  public:
    Archetype(std::string archetypeFile);
    Archetype(std::string archetypeFile, std::string serializedData);
    ~Archetype();
    void Save(std::string& serializedData);
    void Load();
    const std::string& Get();

  private:

    std::string SerializedData;
    std::string SourceFile;
  };

  using ArchetypePtr = std::shared_ptr<Archetype>;

}

