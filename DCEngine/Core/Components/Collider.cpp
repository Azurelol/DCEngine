/******************************************************************************/
/*!
@file   Collider.cpp
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Collider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Collider, "Collider", DCEngineCore, builder, type) {
    DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor
    //ZilchBindConstructor(builder, type, Collider, "owner", Entity&);
    //ZilchBindDestructor(builder, type, Collider);
    // Properties
    //ZilchBindProperty(builder, type, &RigidBody::getDynamicState, &RigidBody::setDynamicState, "DynamicState");
  }
  #endif

  Collider::Collider(Entity & owner, std::string colliderClass) : Component(colliderClass, owner)
  {
  }

  Collider::~Collider()
  {
  }

  void Collider::Initialize()
  {
  }

  
  
  
  
}