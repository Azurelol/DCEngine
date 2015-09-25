#include "BoxCollider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine {
  
  void BoxCollider::Initialize()
  {
    // Subscribe this physics component to the physics space
    space_->getComponent<PhysicsSpace>()->AddCollider(dynamic_cast<GameObject*>(Owner()));
  }

  void BoxCollider::Serialize(Json::Value & root)
  {
  }

  void BoxCollider::Deserialize(Json::Value & root)
  {
  }

}