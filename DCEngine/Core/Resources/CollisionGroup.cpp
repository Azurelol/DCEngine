/*****************************************************************************/
/*!
@file      CollisionGroup.cpp
@author    Blaine Reiner
@par       email: blaine.reiner\@digipen.edu
@date      10/01/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "CollisionGroup.h"
#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(CollisionGroup, "CollisionGroup", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, CollisionGroup ,"collisionGroupFile", std::string);
    ZilchBindDestructor(builder, type, CollisionGroup);
    ZilchBindMethod(builder, type, &MakeCollisionGroup, ZilchNoOverload, "MakeCollisionGroup", "group");
  }
  #endif

  /**************************************************************************/
  /*!
  @brief  CollisionGroup constructor.
  */
  /**************************************************************************/
  CollisionGroup::CollisionGroup(std::string collisionGroupFile) : 
                                Resource("CollisionGroup", FileSystem::FileNoExtension(collisionGroupFile), 
                                collisionGroupFile) 
  {
  }

  /**************************************************************************/
  /*!
  @brief  Returns the texture resource to be used by the graphics system.
  @return A reference to the texture object.
  */
  /**************************************************************************/
  CollisionGroupPtr CollisionGroup::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getCollisionGroup(name);
  }

  CollisionGroup & CollisionGroup::operator=(const CollisionGroup & rhs)
  {
    this->ObjectName = rhs.ObjectName;

    return *this;
  }

  CollisionGroup * MakeCollisionGroup(std::string group)
  {
    return new CollisionGroup(group);
  }

}