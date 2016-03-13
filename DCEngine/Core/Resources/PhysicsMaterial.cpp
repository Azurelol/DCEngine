/*****************************************************************************/
/*!
@file   PhysicsMaterial.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu, blaine.reiner\@digipen.edu
@date   12/17/2015
@brief  Collision groups are used as a tag to identify an object in the table.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PhysicsMaterial.h"
#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(PhysicsMaterial, "PhysicsMaterial", DCEngineCore, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(PhysicsMaterial, Density);
    DCE_BINDING_DEFINE_PROPERTY(PhysicsMaterial, Restitution);
    DCE_BINDING_DEFINE_PROPERTY(PhysicsMaterial, Friction);
  }
  #endif

  PhysicsMaterial::PhysicsMaterial(std::string physicsMaterialFile) : Resource("PhysicsMaterial", 
                                   FileSystem::FileNoExtension(physicsMaterialFile), physicsMaterialFile), Restitution(0)
  {
  }

  PhysicsMaterialPtr PhysicsMaterial::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(name);
  }
}