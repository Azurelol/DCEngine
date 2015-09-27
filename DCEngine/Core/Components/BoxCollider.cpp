#include "BoxCollider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {
  
  void BoxCollider::Initialize()
  {
    auto owner = dynamic_cast<GameObject*>(Owner());
    // Store a reference to the Transform Component
    TransformComponent = owner->getComponent<Transform>();
    // Subscribe this physics component to the physics space
    space_->getComponent<PhysicsSpace>()->AddCollider(owner);
    
  }

  void BoxCollider::Serialize(Json::Value & root)
  {
  }

  void BoxCollider::Deserialize(Json::Value & root)
  {
  }

}