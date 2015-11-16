/*****************************************************************************/
/*!
@file   Archetype.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/14/2015
@brief  An archetype is a resource that contains the serialized data definition
        of an object. The archetype stores a binary cache of the serialization
        data and the source file for debugging and for archetype updating.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "..\Objects\Entities\GameObject.h"

namespace DCEngine {

  class Archetype : public Resource {
  public:
    Archetype(std::string archetypeFile);
    ~Archetype();
    void Save(std::string& serializedData);
    void Load();

  private:

    std::string SerializedData;
    std::string SourceFile;
  };

  using ArchetypePtr = std::shared_ptr<Archetype>;

}

