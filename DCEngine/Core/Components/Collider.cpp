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