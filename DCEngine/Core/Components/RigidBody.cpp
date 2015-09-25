#include "RigidBody.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  void RigidBody::Initialize()
  {    
    auto owner = dynamic_cast<GameObject*>(Owner());
    // Store a reference to the Transform Component
    TransformComponent = owner->getComponent<Transform>();
    // Subscribe this physics component to the physics space
    space_->getComponent<PhysicsSpace>()->AddRigidBody(owner);

  }

  void RigidBody::Serialize(Json::Value & root)
  {

  }

  void RigidBody::Deserialize(Json::Value & root)
  {

  }
}