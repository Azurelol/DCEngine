#include "RigidBody.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  void RigidBody::Initialize()
  {
    // Subscribe this physics component to the physics space
    space_->getComponent<PhysicsSpace>()->AddRigidBody(dynamic_cast<GameObject*>(Owner()));

  }

  void RigidBody::Serialize(Json::Value & root)
  {

  }

  void RigidBody::Deserialize(Json::Value & root)
  {

  }
}